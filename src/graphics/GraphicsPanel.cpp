#include "GraphicsPanel.h"
#include "../style/OptStyle.h"

void GraphicsPanel::setDrawMode(DrawMode mode) { drawMode = mode; }

GraphicsPanel::GraphicsPanel(QWidget* parent)
        : QOpenGLWidget(parent), controller(OpticalController::instance())
{
    setMouseTracking(true);
}

void GraphicsPanel::initializeGL()
{
    setMaximumWidth(controller->width());
    setMaximumHeight(controller->height());
    this->setFocusPolicy(Qt::StrongFocus);
    initializeOpenGLFunctions();
}

void GraphicsPanel::paintGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    pixmap = QPixmap(size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    if (!painter.isActive())
        painter.begin(this);
    glScaled(controller->scale(), controller->scale(), 1.0);

    painter.fillRect(0, 0, width(), height(), OptStyle::HONEYDEW);

    draw_cells(painter);
    draw_axis(painter);

    QPen penObjects(OptStyle::PLAIN_BLACK);
    penObjects.setWidth(5);
    painter.setPen(penObjects);

    if (controller->has_object()) {
        draw_object(painter);
        draw_images(painter);
        draw_image_rays(painter);
    }
    draw_rays(painter);
    for (const Lens &lens : controller->get_lenses())
        draw_lens(lens, painter);

    painter.end();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    QPainter painterGL(this);
    painterGL.drawPixmap(0, 0, pixmap);
    QString results = performCalculations();
    emit calculationsUpdated(results);
}

void GraphicsPanel::draw_cells(QPainter &painter)
{
    QColor cell_color = OptStyle::BERKELEY_BLUE;
    cell_color.setAlpha(100);
    QPen penSystem(cell_color);
    penSystem.setWidth(1);
    painter.setPen(penSystem);

    int maxWidth = 3*width();
    int maxHeight = 3*height();

    int allCellsX = maxWidth / controller->cell_size();
    int midCellsX = width() / controller->cell_size() / 2;
    for (int i = midCellsX; i > midCellsX-allCellsX/2; i--) {
        int x = controller->centerX()
                + (int)((i * controller->cell_size() - controller->centerX()) * controller->scale());
        painter.drawLine(x, 0, x, height());
    }
    for (int i = midCellsX; i < allCellsX; i++) {
        int x = controller->centerX()
                + (int)((i * controller->cell_size() - controller->centerX()) * controller->scale());
        painter.drawLine(x, 0, x, height());
    }

    int allCellsY = maxHeight / controller->cell_size();
    int midCellsY = height() / controller->cell_size() / 2;
    for (int j = midCellsY; j > midCellsY - allCellsY/2; j--) {
        int y = controller->centerY()
                + (int)((j * controller->cell_size() - controller->centerY()) * controller->scale());
        painter.drawLine(0, y, width(), y);
    }
    for (int j = midCellsY; j < allCellsY; j++) {
        int y = controller->centerY()
                + (int)((j * controller->cell_size() - controller->centerY()) * controller->scale());
        painter.drawLine(0, y, width(), y);
    }
}

void GraphicsPanel::draw_lens(const Lens &lens, QPainter &painter)
{
    QPen penLens(OptStyle::CERULEAN_BLUE);
    penLens.setWidth(3);
    painter.setPen(penLens);

    int lensHeight = height() / 2;
    int lensWidth = controller->cell_size() * 2;

    int lensTop = controller->centerY() - lensHeight / 2;
    int lensBottom = controller->centerY() + lensHeight / 2;

    painter.drawLine(controller->screenX(lens.x()),
                      lensTop,
                      controller->screenX(lens.x()) - lensWidth / 2,
                      lensTop + (lens.isConverging() ? 10 : -10));
    painter.drawLine(controller->screenX(lens.x()) + lensWidth / 2,
                      lensTop + (lens.isConverging() ? 10 : -10),
                      controller->screenX(lens.x()),
                      lensTop);

    painter.drawLine(controller->screenX(lens.x()),
                      lensBottom,
                      controller->screenX(lens.x()) - lensWidth / 2,
                      lensBottom + (lens.isConverging() ? -10 : 10));
    painter.drawLine(controller->screenX(lens.x()) + lensWidth / 2,
                      lensBottom + (lens.isConverging() ? -10 : 10),
                      controller->screenX(lens.x()),
                      lensBottom);

    painter.drawLine(controller->screenX(lens.x()),
                      lensTop,
                      controller->screenX(lens.x()),
                      lensBottom);

    QPen penFoci(OptStyle::RED_PANTONE);
    penFoci.setWidth(5);
    painter.setPen(penFoci);

    Point front = controller->screenPoint(Point(lens.x() - lens.getFocusLength(), 0));
    Point back = controller->screenPoint(Point(lens.x() + lens.getFocusLength(), 0));

    painter.drawPoint(front.x(), front.y());
    painter.drawPoint(back.x(), back.y());
}

void GraphicsPanel::draw_axis(QPainter &painter) {
    QPen penSystem(OptStyle::BERKELEY_BLUE);
    penSystem.setWidth(3);
    painter.setPen(penSystem);
    int optAxis = controller->height() / 2;
    painter.drawLine(0, optAxis, width(), optAxis);
}

void GraphicsPanel::draw_object(QPainter &painter) {
    QPen penObjects(OptStyle::RED_PANTONE);
    penObjects.setWidth(6);
    painter.setPen(penObjects);
    Segment line = controller->screenSegment(controller->get_object());
    painter.drawLine(line.startX(), line.startY(), line.endX(), line.endY());
}

void GraphicsPanel::draw_images(QPainter &painter) {
    QPen penObjects(OptStyle::NON_PHOTO_BLUE.darker(150));
    penObjects.setWidth(4);
    painter.setPen(penObjects);
    for (Segment line : controller->get_images()) {
        Segment translated = controller->screenSegment(line);
        painter.drawLine(translated.startX(), translated.startY(), translated.endX(), translated.endY());
    }
}

void GraphicsPanel::draw_image_rays(QPainter &painter) {
    QPen penObjects(OptStyle::SILVER);
    penObjects.setWidth(2);
    painter.setPen(penObjects);
    for (Segment line : controller->get_image_rays()) {
        Segment translated = controller->screenSegment(line);
        painter.drawLine(translated.startX(), translated.startY(), translated.endX(), translated.endY());
    }
}

void GraphicsPanel::draw_rays(QPainter &painter) {
    QPen penObjects(OptStyle::ORANGE_WEB);
    penObjects.setWidth(4);
    painter.setPen(penObjects);
    for (Segment ray : controller->get_rays()) {
        int scaledX1 = controller->screenX(ray.startX());
        int scaledY1 = controller->screenY(ray.startY());
        int scaledX2 = controller->screenX(ray.endX());
        int scaledY2 = controller->screenY(ray.endY());
        int arrowLength = 10;
        int dx = scaledX2 - scaledX1;
        int dy = scaledY2 - scaledY1;
        double angle = atan2(dy, dx);
        int arrowX1 = scaledX2 - arrowLength * cos(angle + M_PI / 4);
        int arrowY1 = scaledY2 - arrowLength * sin(angle + M_PI / 4);
        int arrowX2 = scaledX2 - arrowLength * cos(angle - M_PI / 4);
        int arrowY2 = scaledY2 - arrowLength * sin(angle - M_PI / 4);
        painter.drawLine(QPoint(scaledX1, scaledY1), QPointF(scaledX2, scaledY2));
        painter.drawLine(QPoint(scaledX2, scaledY2), QPoint(arrowX1, arrowY1));
        painter.drawLine(QPoint(scaledX2, scaledY2), QPoint(arrowX2, arrowY2));
    }
}

void GraphicsPanel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // get position
        QPoint pos = event->pos();
        if (drawMode == DrawMode::Point)
            controller->set_object(controller->getX(pos.x()), controller->getY(pos.y()));
        else if (drawMode == DrawMode::Ray) {
            if (!startPointSet) {
                startPoint = pos;
                startPointSet = true;
            } else { // if double click - draw line
                Point start(controller->getX(startPoint.x()), controller->getY(startPoint.y()));
                Point end(controller->getX(pos.x()), controller->getY(pos.y()));
                controller->add_ray(Segment(start, end));
                startPointSet = false;
            }
        }
        update();
    }
}

void GraphicsPanel::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == (Qt::ControlModifier | Qt::KeypadModifier)) {
        switch(event->key()) {
            case Qt::Key_Plus:
                controller->scale() *= 1.2;
                if (controller->scale() > 3.0)
                    controller->scale() = 3.0;
                update();
                break;
            case Qt::Key_Minus:
                controller->scale() /= 1.2;
                if (controller->scale() < 0.4)
                    controller->scale() = 0.4;
                qDebug() << "_scale:" << controller->scale();
                update();
                break;
            default:
                QWidget::keyPressEvent(event);
                break;
        }
    }
}

void GraphicsPanel::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        int numDegrees = event->angleDelta().y() / 8;
        float numSteps = static_cast<float>(numDegrees) / 15.0f;
        controller->scale() *= qPow(1.125f, numSteps);
        if (controller->scale() > 3.0)
            controller->scale() = 3.0;
        else if (controller->scale() < 0.4)
            controller->scale() = 0.4;
        update();
    }
}

void GraphicsPanel::clearPanel()
{
    controller->clear_all();
    update();
}

void GraphicsPanel::clearGeometry() {
    controller->clear_geometry();
    update();
}

void GraphicsPanel::saveModel()
{
    QString filePath =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Image"), "",
                                         tr("Images (*.png *.xpm *.jpg)"));
    if (!filePath.isEmpty()) {
        pixmap.save(filePath);
    }
}

void GraphicsPanel::addLens() {
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Додати лінзу"));
    QFormLayout formLayout(&dialog);

    QDoubleSpinBox powerSpinBox;
    powerSpinBox.setRange(-100, 100);
    QDoubleSpinBox coordSpinBox;
//    coordSpinBox.setRange(-this->width() / controller->cell_size(), this->width() / controller->cell_size());

    formLayout.addRow(tr("Оптична сила:"), &powerSpinBox);
    formLayout.addRow(tr("Координата x:"), &coordSpinBox);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok);
    formLayout.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, [&]() {
        if (powerSpinBox.value() == 0.0) {
            QMessageBox::critical(this, tr("Помилка!"),
                                  tr("Оптична сила лінзи не може бути рівна 0."));
            return;
        }
        // or lens with coordinate coordSpinBox.value() is already exists?
        else if (controller->lens_on_x_exists(coordSpinBox.value())) {
            QMessageBox::critical(this, tr("Помилка!"),
                                  tr("Лінза з такою координатою уже існує."));
            return;
        }
        dialog.accept();
    });

    if (dialog.exec() == QDialog::Accepted) {
        double power = powerSpinBox.value();
        double x = coordSpinBox.value();
        controller->add_lens(Lens(power, x));
        update();
    }
}

void GraphicsPanel::addObject() {
    // TODO unify QDialog
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Додати об'єкт"));
    QFormLayout formLayout(&dialog);

    QComboBox measure;
    formLayout.addRow(tr("Одиниця вимірювання:"), &measure);
    measure.addItem("м");
    measure.addItem("см");
    measure.addItem("мм");
    measure.addItem("км");
    QDoubleSpinBox spinBox1;
    QDoubleSpinBox spinBox2;
    spinBox1.setRange(std::numeric_limits<double>::lowest(),std::numeric_limits<double>::infinity());
    spinBox2.setRange(std::numeric_limits<double>::lowest(),std::numeric_limits<double>::infinity());

    spinBox1.setRange(-INFINITY, INFINITY);
    spinBox2.setRange(-INFINITY, INFINITY);
    formLayout.addRow(tr("Висота об'єкта:"), &spinBox1);
    formLayout.addRow(tr("Координата x:"), &spinBox2);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok);
    formLayout.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        double height = -spinBox1.value();
        double x = spinBox2.value();
        controller->set_object(OpticalController::convert_to_meter(x, measure.currentText()),
                               OpticalController::convert_to_meter(height, measure.currentText()));
        update();
    }
}

void GraphicsPanel::setCellScale() {
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Задайте розмір клітинки"));
    QFormLayout formLayout(&dialog);

    QComboBox measure;
    QDoubleSpinBox cell_scale;

    formLayout.addRow(tr("Одиниця вимірювання:"), &measure);
    measure.addItem("м");
    measure.addItem("см");
    measure.addItem("мм");
    measure.addItem("км");
    formLayout.addRow(tr("Розмірність:"), &cell_scale);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok);
    formLayout.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, [&]() {
        if (cell_scale.value() == 0.0) {
            QMessageBox::critical(this, tr("Помилка!"),
                                  tr("Розмірність клітинки не може бути рівна 0."));
            return;
        }
        dialog.accept();
    });

    if (dialog.exec() == QDialog::Accepted) {
        QString res_measure = measure.currentText();
        double res_scale = cell_scale.value();
        controller->cell_scale() = OpticalController::convert_to_meter(res_scale, res_measure);
        update();
    }
}

QString GraphicsPanel::performCalculations() {
    QString results = "";

    int lens_counter = 1;
    if(!controller->get_lenses().empty()) {
        results += "Система лінз:\n";
    }
    for(Lens lens : controller->get_lenses()) {
        results += "D" + QString::number(lens_counter++) + "="
                   + QString::number(lens.optPow()) + ", F=" + QString::number(lens.getFocusLength()) + "(м)\n";
    }

    if(controller->has_object()) {
        results += "\nОб'єкт:\n";
        results += "h=" + QString::number(-controller->get_object().startY()) + "\n";
        results += "x: " + QString::number(controller->get_object().startX()) + "\n";
        int image_counter = 1;
        results += "\nЗображення:\n";
        for(Segment image : controller->get_images()) {
            results += QString::number(image_counter++) + ". ";
            results += "H=" +
                       QString::number(-image.startY()) + ", ";
            if(abs(image.startY()) > abs(controller->get_object().startY())) {
                results += "збільшене ";
            }
            else {
                results += "зменшене ";
            }
            if((std::next((controller->objectImages()).begin(), image_counter))->is_real()) {
                results += "дійсне.\n";
            }
            else {
                results += "уявне.\n";
            }
            results += "x: " + QString::number(image.startX()) + "\n";
        }

    }
    results += "\nРозмірність клітинки: " + QString::number(controller->cell_scale()) + " м\n";
    results += "Ширина екрану: " + QString::number((controller->cell_scale() * 50) / controller->scale()) + " м";
    return results;
}
