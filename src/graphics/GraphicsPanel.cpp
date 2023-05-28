//
// Created by Lilly on 28.05.2023.
//

#include "GraphicsPanel.h"

void GraphicsPanel::initializeGL() {
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

    // TODO: add scale

    painter.fillRect(0, 0, width, height, QColor(241, 250, 238));

    cellSize = (width + height)/60;

    int cellNumberX = width / cellSize;
    int cellNumberY = height / cellSize;

    QPen penSystem(QColor(29, 53, 87, 100));
    penSystem.setWidth(1);

    // TODO fix step
    int xStep = qAbs(width - (cellSize*cellNumberX))/2;
    int yStep = qAbs(height - (cellSize*cellNumberY))/2;

    for (int x = -1; x < cellNumberX + 1; ++x) {
        for (int y = -1; y < cellNumberY + 1; ++y) {
            int xPos = x * cellSize + xStep;
            int yPos = y * cellSize + yStep;

            painter.fillRect(xPos, yPos, cellSize, cellSize, QColor(241, 250, 238));

            painter.setPen(penSystem);
            painter.drawRect(xPos, yPos, cellSize, cellSize);
        }
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
        painter.drawPoint(point);
    }

    for (const auto& line : lines) {
        painter.drawLine(line.first, line.second);
    }

    painter.end();
}

void GraphicsPanel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // get position
        QPointF pos = event->position();

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