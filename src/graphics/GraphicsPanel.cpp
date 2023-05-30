//
// Created by Lilly on 28.05.2023.
//

#include "GraphicsPanel.h"

void GraphicsPanel::initializeGL() {
    setMaximumWidth(WIDTH);
    setMaximumHeight(HEIGHT);
    this->setFocusPolicy(Qt::StrongFocus);
    scaleFactor = 1.0f;
    initializeOpenGLFunctions();
}

void GraphicsPanel::paintGL()  {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    pixmap = QPixmap(size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.begin(this);

    int width = this->width();
    int height = this->height();

    int centerX = width / 2;
    int centerY = height / 2;
    float offsetX = static_cast<float>(centerX) * (1.0f - scaleFactor);
    float offsetY = static_cast<float>(centerY) * (1.0f - scaleFactor);

    glTranslatef(offsetX, offsetY, 0.0f);
    glScalef(scaleFactor, scaleFactor, 1.0f);
    glTranslatef(-offsetX, -offsetY, 0.0f);

    painter.fillRect(0, 0, width, height, QColor(241, 250, 238));

    QPen penSystem(QColor(29, 53, 87, 100));
    penSystem.setWidth(1);
    painter.setPen(penSystem);

    int maxWidth = 3*width;
    int maxHeight = 3*height;

    // drawing cells
    cellSize = height / 30;
    int allCellsX = maxWidth / cellSize;
    int midCellsX = width / cellSize / 2;
    for (int i = midCellsX; i > midCellsX-allCellsX/2; i--) {
        int x = centerX + (i * cellSize - centerX) * scaleFactor;
        painter.drawLine(x, 0, x, height);
    }
    for (int i = midCellsX; i < allCellsX; i++) {
        int x = centerX + (i * cellSize - centerX) * scaleFactor;
        painter.drawLine(x, 0, x, height);
    }

    int allCellsY = maxHeight / cellSize;
    int midCellsY = height / cellSize / 2;
    for (int j = midCellsY; j > midCellsY - allCellsY/2; j--) {
        int y = centerY + (j * cellSize - centerY) * scaleFactor;
        painter.drawLine(0, y, width, y);
    }
    for (int j = midCellsY; j < allCellsY; j++) {
        int y = centerY + (j * cellSize - centerY) * scaleFactor;
        painter.drawLine(0, y, width, y);
    }

    // optical axis
    penSystem.setColor(QColor(29, 53, 87));
    penSystem.setWidth(3);
    painter.setPen(penSystem);
    int optAxis = static_cast<int>(height / 2);
    painter.drawLine(0, optAxis, width, optAxis);

    QPen penObjects(Qt::black);
    penObjects.setWidth(5);
    painter.setPen(penObjects);

    // all drawn points
    for (const auto& point : points) {
        int scaledX = centerX + (point.x() - centerX) * scaleFactor;
        int scaledY = centerY + (point.y() - centerY) * scaleFactor;
        painter.drawPoint(QPoint(scaledX, scaledY));
    }
    penObjects.setWidth(2);
    painter.setPen(penObjects);
    // all drawn lines
    for (const auto& line : lines) {
        int scaledX1 = centerX + (line.first.x() - centerX) * scaleFactor;
        int scaledY1 = centerY + (line.first.y() - centerY) * scaleFactor;
        int scaledX2 = centerX + (line.second.x() - centerX) * scaleFactor;
        int scaledY2 = centerY + (line.second.y() - centerY) * scaleFactor;
        painter.drawLine(QPoint(scaledX1, scaledY1), QPointF(scaledX2, scaledY2));
    }

    // all drawn rays
    for (const auto& ray : rays) {
        int scaledX1 = centerX + (ray.first.x() - centerX) * scaleFactor;
        int scaledY1 = centerY + (ray.first.y() - centerY) * scaleFactor;
        int scaledX2 = centerX + (ray.second.x() - centerX) * scaleFactor;
        int scaledY2 = centerY + (ray.second.y() - centerY) * scaleFactor;

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

    // TODO: if Lens != NULL ?????
    if (lensPower != 0) {
        QPen penLens(QColor("#457B9D"));
        penLens.setWidth(3);
        painter.setPen(penLens);

        int lensHeight = this->height() / 2;
        int lensWidth = cellSize * 2;

        int lensTop = centerY - lensHeight / 2;
        int lensBottom = centerY + lensHeight / 2;

        bool isConv = lensPower > 0;

        painter.drawLine(centerX - lensWidth / 2, lensTop, centerX, lensTop + (isConv ? -10 : 10));
        painter.drawLine(centerX, lensTop + (isConv ? -10 : 10), centerX + lensWidth / 2, lensTop);

        painter.drawLine(centerX - lensWidth / 2, lensBottom, centerX, lensBottom + (isConv ? 10 : -10));
        painter.drawLine(centerX, lensBottom + (isConv ? 10 : -10), centerX + lensWidth / 2, lensBottom);

        painter.drawLine(centerX, lensTop, centerX, lensBottom);
    }

    if (focalLengthFront != 0 && focalLengthBack != 0) {
        QPen penFoci(QColor("#E63946"));
        penFoci.setWidth(5);
        painter.setPen(penFoci);

        QPoint front = getCoordinates(focalLengthFront, 0);
        QPoint back = getCoordinates(focalLengthBack, 0);

        painter.drawPoint(front);
        painter.drawPoint(back);
    }

    painter.end();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    QPainter painterGL(this);
    painterGL.drawPixmap(0, 0, pixmap);
}

void GraphicsPanel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // get position
        QPoint pos = event->pos();
        int centerX = width() / 2;
        int centerY = height() / 2;

        // TODO fix for all scales
        float scaledCellSize = static_cast<float>(cellSize) * scaleFactor;
        int cellX = qRound((pos.x() - centerX) / scaledCellSize);
        int cellY = qRound((pos.y() - centerY) / scaledCellSize);
        int x = centerX + cellX * scaledCellSize;
        int y = centerY + cellY * scaledCellSize;

        // int scaledX = (centerX + (pos.x() - centerX) / scaleFactor);
        // int scaledY = (centerY + (pos.y() - centerY) / scaleFactor);
        pos.setX(x);
        pos.setY(y);

        if (drawMode == DrawMode::Point) {
            points.push_back(pos);
        }
        else if (drawMode == DrawMode::Line || drawMode == DrawMode::Ray) {
            if (!startPointSet) {
                startPoint = pos;
                startPointSet = true;
            } else { // if double click - draw line
                if(drawMode == DrawMode::Line)
                    lines.emplace_back(startPoint, pos);
                else
                    rays.emplace_back(startPoint, pos);
                startPointSet = false;
            }
        }

        update();
    }
}

void GraphicsPanel::keyPressEvent(QKeyEvent *event) {
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

void GraphicsPanel::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() == Qt::ControlModifier) {
        float numDegrees = event->angleDelta().y() / 8.0f;
        float numSteps = numDegrees / 15.0f;
        scaleFactor *= qPow(1.125f, numSteps);
        if (scaleFactor > 3.0)
            scaleFactor = 3.0;
        else if (scaleFactor < 0.4)
            scaleFactor = 0.4;
        update();
    }
}

void GraphicsPanel::clearPanel() {
    points.clear();
    lines.clear();
    update();
}

void GraphicsPanel::saveModel() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png *.xpm *.jpg)"));
    if (!filePath.isEmpty()) {
        pixmap.save(filePath);
    }
}

// TODO: args Point
QPoint GraphicsPanel::getCoordinates(double x, double y) {
    int centerX = width() / 2;
    int centerY = height() / 2;
    int newX = (static_cast<int>(x)*cellSize + centerX); // TODO fix for all scales
    int newY = (static_cast<int>(y)*cellSize + centerY);
    return QPoint(newX, newY);
}

void GraphicsPanel::addLens() {
    bool ok;
    float power = QInputDialog::getDouble(this, tr("Add Lens"), tr("Enter lens power:"), 0.0, -100.0, 100.0, 2, &ok);
    if (ok) {
        lensPower = power;
        // TODO change code to add Lens
        if (lensPower != 0.0f) {
            focalLengthFront = qAbs(1 / lensPower);
            focalLengthBack = -focalLengthFront;
        } else {
            focalLengthFront = 0.0f;
            focalLengthBack = 0.0f;
        }
        update();
    }
}