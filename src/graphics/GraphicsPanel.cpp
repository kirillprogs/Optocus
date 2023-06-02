#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "GraphicsPanel.h"

void GraphicsPanel::setDrawMode(DrawMode mode) { drawMode = mode; }

int GraphicsPanel::centerX() { return width() / 2; }
int GraphicsPanel::centerY() { return height() / 2; }

GraphicsPanel::GraphicsPanel(QWidget* parent)
        : QOpenGLWidget(parent), controller(OpticalController::instance())
        , cellSize(0), scaleFactor(1.f)
{
    setMouseTracking(true);
}

void GraphicsPanel::initializeGL()
{
    setMaximumWidth(WIDTH);
    setMaximumHeight(HEIGHT);
    this->setFocusPolicy(Qt::StrongFocus);
    scaleFactor = 1.0f;
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

    float offsetX = static_cast<float>(centerX()) * (1.0f - scaleFactor);
    float offsetY = static_cast<float>(centerY()) * (1.0f - scaleFactor);

    glTranslatef(offsetX, offsetY, 0.0f);
    glScalef(scaleFactor, scaleFactor, 1.0f);
    glTranslatef(-offsetX, -offsetY, 0.0f);

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

void GraphicsPanel::draw_cells(QPainter &painter) {
    QPen penSystem(QColor(29, 53, 87, 100));
    penSystem.setWidth(1);
    painter.setPen(penSystem);

    int maxWidth = 3*width();
    int maxHeight = 3*height();

    cellSize = height() / CELL_NUM;
    int allCellsX = maxWidth / cellSize;
    int midCellsX = width() / cellSize / 2;
    for (int i = midCellsX; i > midCellsX-allCellsX/2; i--) {
        int x = centerX() + (i * cellSize - centerX()) * scaleFactor;
        painter.drawLine(x, 0, x, height());
    }
    for (int i = midCellsX; i < allCellsX; i++) {
        int x = centerX() + (i * cellSize - centerX()) * scaleFactor;
        painter.drawLine(x, 0, x, height());
    }

    int allCellsY = maxHeight / cellSize;
    int midCellsY = height() / cellSize / 2;
    for (int j = midCellsY; j > midCellsY - allCellsY/2; j--) {
        int y = centerY() + (j * cellSize - centerY()) * scaleFactor;
        painter.drawLine(0, y, width(), y);
    }
    for (int j = midCellsY; j < allCellsY; j++) {
        int y = centerY() + (j * cellSize - centerY()) * scaleFactor;
        painter.drawLine(0, y, width(), y);
    }
}

void GraphicsPanel::draw_lens(const Lens &lens, QPainter &painter)
{
    QPen penLens(QColor("#457B9D"));
    penLens.setWidth(3);
    painter.setPen(penLens);

    int lensHeight = height() / 2;
    int lensWidth = cellSize * 2;

    int lensTop = centerY() - lensHeight / 2;
    int lensBottom = centerY() + lensHeight / 2;

    painter.drawLine(getCoordinates(lens.x(), 0).x() - lensWidth / 2,
                      lensTop,
                      getCoordinates(lens.x(), 0).x(),
                      lensTop + (lens.isConverging() ? -10 : 10));
    painter.drawLine(getCoordinates(lens.x(), 0).x(),
                      lensTop + (lens.isConverging() ? -10 : 10),
                      getCoordinates(lens.x(), 0).x() + lensWidth / 2,
                      lensTop);

    painter.drawLine(getCoordinates(lens.x(), 0).x() - lensWidth / 2,
                      lensBottom,
                      getCoordinates(lens.x(), 0).x(),
                      lensBottom + (lens.isConverging() ? 10 : -10));
    painter.drawLine(getCoordinates(lens.x(), 0).x(),
                      lensBottom + (lens.isConverging() ? 10 : -10),
                      getCoordinates(lens.x(), 0).x() + lensWidth / 2,
                      lensBottom);

    painter.drawLine(getCoordinates(lens.x(), 0).x(),
                      lensTop,
                      getCoordinates(lens.x(), 0).x(),
                      lensBottom);

    QPen penFoci(QColor("#E63946"));
    penFoci.setWidth(5);
    painter.setPen(penFoci);

    QPoint front = getCoordinates(lens.x() - lens.getFocusLength(), 0);
    QPoint back = getCoordinates(lens.x() + lens.getFocusLength(), 0);

    painter.drawPoint(front);
    painter.drawPoint(back);
}

void GraphicsPanel::draw_axis(QPainter &painter) {
    QPen penSystem(QColor(29, 53, 87));
    penSystem.setWidth(3);
    painter.setPen(penSystem);
    int optAxis = static_cast<int>(height() / 2);
    painter.drawLine(0, optAxis, width(), optAxis);
}

void GraphicsPanel::draw_object(QPainter &painter) {
    QPen penObjects(Qt::red);
    penObjects.setWidth(6);
    painter.setPen(penObjects);
    Segment line = controller->get_object();
    int scaledX1 = centerX() + line.start().x() * scaleFactor;
    int scaledY1 = centerY() + line.start().y() * scaleFactor;
    int scaledX2 = centerX() + line.end().x() * scaleFactor;
    int scaledY2 = centerY() + line.end().y() * scaleFactor;
    painter.drawLine(QPoint(scaledX1, scaledY1), QPointF(scaledX2, scaledY2));
}

void GraphicsPanel::draw_images(QPainter &painter) {
    QPen penObjects(Qt::darkCyan);
    penObjects.setWidth(4);
    painter.setPen(penObjects);
    for (Segment line : controller->get_images()) {
        int scaledX1 = centerX() + line.start().x() * scaleFactor;
        int scaledY1 = centerY() + line.start().y() * scaleFactor;
        int scaledX2 = centerX() + line.end().x() * scaleFactor;
        int scaledY2 = centerY() + line.end().y() * scaleFactor;
        painter.drawLine(QPoint(scaledX1, scaledY1), QPointF(scaledX2, scaledY2));
    }
}

void GraphicsPanel::draw_image_rays(QPainter &painter) {
    QPen penObjects(Qt::black);
    penObjects.setWidth(2);
    painter.setPen(penObjects);
    for (Segment line : controller->get_image_rays()) {
        int scaledX1 = centerX() + line.start().x() * scaleFactor;
        int scaledY1 = centerY() + line.start().y() * scaleFactor;
        int scaledX2 = centerX() + line.end().x() * scaleFactor;
        int scaledY2 = centerY() + line.end().y() * scaleFactor;
        painter.drawLine(QPoint(scaledX1, scaledY1), QPointF(scaledX2, scaledY2));
    }
}

void GraphicsPanel::draw_rays(QPainter &painter) {
    QPen penObjects(Qt::yellow);
    penObjects.setWidth(4);
    painter.setPen(penObjects);
    for (Segment ray : controller->get_rays()) {
        int scaledX1 = centerX() + ray.start().x() * scaleFactor;
        int scaledY1 = centerY() + ray.start().y() * scaleFactor;
        int scaledX2 = centerX() + ray.end().x() * scaleFactor;
        int scaledY2 = centerY() + ray.end().y() * scaleFactor;

        // arrow for ray
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

void GraphicsPanel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // get position
        QPoint pos = event->pos();

        // TODO fix for all scales
        float scaledCellSize = static_cast<float>(cellSize) * scaleFactor;
        int cellX = qRound(static_cast<float>(pos.x() - centerX()) / scaledCellSize);
        int cellY = qRound(static_cast<float>(pos.y() - centerY()) / scaledCellSize);
        int x = cellX * cellSize;
        int y = cellY * cellSize;
        pos.setX(x);
        pos.setY(y);

        if (drawMode == DrawMode::Point) {
            controller->set_object(pos.x(), pos.y());
        }
        else if (drawMode == DrawMode::Line || drawMode == DrawMode::Ray) {
            if (!startPointSet) {
                startPoint = pos;
                startPointSet = true;
            } else { // if double click - draw line
                if(drawMode == DrawMode::Line)
                    rays.emplace_back(startPoint, pos);
                else
                    rays.emplace_back(startPoint, pos);
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
                scaleFactor *= 1.2;
                if (scaleFactor > 3.0)
                    scaleFactor = 3.0;
                update();
                break;
            case Qt::Key_Minus:
                scaleFactor /= 1.2;
                if (scaleFactor < 0.4)
                    scaleFactor = 0.4;
                qDebug() << "scale:" << scaleFactor;
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
        scaleFactor *= qPow(1.125f, numSteps);
        if (scaleFactor > 3.0)
            scaleFactor = 3.0;
        else if (scaleFactor < 0.4)
            scaleFactor = 0.4;
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
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png *.xpm *.jpg)"));
    if (!filePath.isEmpty()) {
        pixmap.save(filePath);
    }
}

// TODO: args Point
QPoint GraphicsPanel::getCoordinates(double x, double y)
{
    int newX = (static_cast<int>(x*cellSize * scaleFactor) + centerX());
    int newY = (static_cast<int>(y*cellSize * scaleFactor) + centerY());
    return { newX, newY };
}



void GraphicsPanel::addLens() {
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Додати лінзу"));
    QFormLayout formLayout(&dialog);

    QDoubleSpinBox powerSpinBox;
    powerSpinBox.setRange(-10, 10);
    QSpinBox coordSpinBox;
    coordSpinBox.setRange(-this->width() / cellSize, this->width() / cellSize);

    formLayout.addRow(tr("Оптична сила:"), &powerSpinBox);
    formLayout.addRow(tr("Координата x:"), &coordSpinBox);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok);
    formLayout.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, [&]() {
        if (powerSpinBox.value() == 0.0) {
            QMessageBox::critical(this, tr("Помилка!"), tr("Оптична сила лінзи не може бути рівна 0."));
            return;
        }
        // or lens with coordinate coordSpinBox.value() is already exists?
        dialog.accept();
    });

    if (dialog.exec() == QDialog::Accepted) {
        double power = powerSpinBox.value();
        int x = coordSpinBox.value();
        controller->add_lens(Lens(power, x));
        update();
    }
}

void GraphicsPanel::addObject() {
    // TODO unify QDialog
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Додати об'єкт"));
    QFormLayout formLayout(&dialog);

    QSpinBox spinBox1;
    spinBox1.setRange(-INT_MAX, INT_MAX);
    QSpinBox spinBox2;
    spinBox2.setRange(-this->width() / cellSize, this->width() / cellSize);

    formLayout.addRow(tr("Висота об'єкта:"), &spinBox1);
    formLayout.addRow(tr("Координата x:"), &spinBox2);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok);
    formLayout.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        double height = spinBox1.value();
        int x = spinBox2.value();
        QPoint obj = getCoordinates(x, height);
        float scaledCellSize = static_cast<float>(cellSize) * scaleFactor;
        int cellX = qRound(static_cast<float>(obj.x() - centerX()) / scaledCellSize);
        int cellY = qRound(static_cast<float>(obj.y() - centerY()) / scaledCellSize);
        controller->set_object(cellX * cellSize, -cellY * cellSize);
        update();
    }
}