//
// Created by Lilly on 28.05.2023.
//

#ifndef OPTOCUS_GRAPHICSPANEL_H
#define OPTOCUS_GRAPHICSPANEL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>

class GraphicsPanel : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GraphicsPanel(QWidget* parent = nullptr) : QOpenGLWidget(parent) {
            setMouseTracking(true);
    }
    enum class DrawMode {
        Point,
        Line
    };

    void setDrawMode(DrawMode mode) {
        drawMode = mode;
    }
protected:
    void initializeGL() override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    int cellSize;
    QVector<QPointF> points;
    QVector<QPair<QPointF, QPointF>> lines;
    QPointF startPoint;
    bool startPointSet = false;
    DrawMode drawMode = DrawMode::Point;

};


#endif //OPTOCUS_GRAPHICSPANEL_H
