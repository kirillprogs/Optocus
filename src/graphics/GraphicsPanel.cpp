#include "GraphicsPanel.h"

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

    painter.fillRect(0, 0, width(), height(), QColor(241, 250, 238));

    draw_cells(painter);
    draw_axis(painter);

    QPen penObjects(Qt::black);
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
}

void GraphicsPanel::draw_cells(QPainter &painter)
{
    QPen penSystem(QColor(29, 53, 87, 100));
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
    QPen penLens(QColor("#457B9D"));
    penLens.setWidth(3);
    painter.setPen(penLens);

    int lensHeight = height() / 2;
    int lensWidth = controller->cell_size() * 2;

    int lensTop = controller->centerY() - lensHeight / 2;
    int lensBottom = controller->centerY() + lensHeight / 2;

    painter.drawLine(controller->screenX(lens.x()) - lensWidth / 2,
                      lensTop,
                      controller->screenX(lens.x()),
                      lensTop + (lens.isConverging() ? -10 : 10));
    painter.drawLine(controller->screenX(lens.x()),
                      lensTop + (lens.isConverging() ? -10 : 10),
                      controller->screenX(lens.x()) + lensWidth / 2,
                      lensTop);

    painter.drawLine(controller->screenX(lens.x()) - lensWidth / 2,
                      lensBottom,
                      controller->screenX(lens.x()),
                      lensBottom + (lens.isConverging() ? 10 : -10));
    painter.drawLine(controller->screenX(lens.x()),
                      lensBottom + (lens.isConverging() ? 10 : -10),
                      controller->screenX(lens.x()) + lensWidth / 2,
                      lensBottom);

    painter.drawLine(controller->screenX(lens.x()),
                      lensTop,
                      controller->screenX(lens.x()),
                      lensBottom);

    QPen penFoci(QColor("#E63946"));
    penFoci.setWidth(5);
    painter.setPen(penFoci);

    Point front = controller->screenPoint(Point(lens.x() - lens.getFocusLength(), 0));
    Point back = controller->screenPoint(Point(lens.x() + lens.getFocusLength(), 0));

    painter.drawPoint(front.x(), front.y());
    painter.drawPoint(back.x(), back.y());
}

void GraphicsPanel::draw_axis(QPainter &painter) {
    QPen penSystem(QColor(29, 53, 87));
    penSystem.setWidth(3);
    painter.setPen(penSystem);
    int optAxis = controller->height() / 2;
    painter.drawLine(0, optAxis, width(), optAxis);
}

void GraphicsPanel::draw_object(QPainter &painter) {
    QPen penObjects(Qt::red);
    penObjects.setWidth(6);
    painter.setPen(penObjects);
    Segment line = controller->screenSegment(controller->get_object());
    painter.drawLine(line.startX(), line.startY(), line.endX(), line.endY());
}

void GraphicsPanel::draw_images(QPainter &painter) {
    QPen penObjects(Qt::darkCyan);
    penObjects.setWidth(4);
    painter.setPen(penObjects);
    for (Segment line : controller->get_images()) {
        Segment translated = controller->screenSegment(line);
        painter.drawLine(translated.startX(), translated.startY(), translated.endX(), translated.endY());
    }
}

void GraphicsPanel::draw_image_rays(QPainter &painter) {
    QPen penObjects(Qt::lightGray);
    penObjects.setWidth(2);
    painter.setPen(penObjects);
    for (Segment line : controller->get_image_rays()) {
        Segment translated = controller->screenSegment(line);
        painter.drawLine(translated.startX(), translated.startY(), translated.endX(), translated.endY());
    }
}

void GraphicsPanel::draw_rays(QPainter &painter) {
    QPen penObjects(Qt::yellow);
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
//    powerSpinBox.setRange(-10, 10);
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

    QDoubleSpinBox spinBox1;
    QDoubleSpinBox spinBox2;

    formLayout.addRow(tr("Висота об'єкта:"), &spinBox1);
    formLayout.addRow(tr("Координата x:"), &spinBox2);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok);
    formLayout.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        double height = spinBox1.value();
        double x = spinBox2.value();
        controller->set_object(x, height);
        update();
    }
}