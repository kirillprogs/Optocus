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

    QPainter painter;
    painter.begin(this);

    int width = this->width();
    int height = this->height();
    qDebug() << "width=" << width;
    qDebug() << "height=" << height;

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

    penSystem.setColor(QColor(29, 53, 87));
    penSystem.setWidth(3);
    painter.setPen(penSystem);
    int optAxis = static_cast<int>(height / 2);
    painter.drawLine(0, optAxis, width, optAxis);

    QPen penObjects(Qt::black);
    penObjects.setWidth(2);
    painter.setPen(penObjects);

    for (const auto& point : points) {
        int scaledX = centerX + (point.x() - centerX) * scaleFactor;
        int scaledY = centerY + (point.y() - centerY) * scaleFactor;
        painter.drawPoint(QPointF(scaledX, scaledY));
    }

    for (const auto& line : lines) {
        int scaledX1 = centerX + (line.first.x() - centerX) * scaleFactor;
        int scaledY1 = centerY + (line.first.y() - centerY) * scaleFactor;
        int scaledX2 = centerX + (line.second.x() - centerX) * scaleFactor;
        int scaledY2 = centerY + (line.second.y() - centerY) * scaleFactor;
        painter.drawLine(QPointF(scaledX1, scaledY1), QPointF(scaledX2, scaledY2));
    }

    painter.end();
}

void GraphicsPanel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // get position
        QPointF pos = event->position();
        int centerX = this->width() / 2;
        int centerY = this->height() / 2;
        int scaledX = (centerX + (pos.x() - centerX) / scaleFactor);
        int scaledY = (centerY + (pos.y() - centerY) / scaleFactor);
        pos.setX(scaledX);
        pos.setY(scaledY);

        if (drawMode == DrawMode::Point) {
            points.push_back(pos);
        }
        else if (drawMode == DrawMode::Line) {
            if (!startPointSet) {
                startPoint = pos;
                startPointSet = true;
            } else { // if double click - draw line
                lines.emplace_back(startPoint, pos);
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