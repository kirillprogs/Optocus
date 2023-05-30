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
#include <QFileDialog>

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
    void clearPanel();

    void saveModel();
protected:
    float scaleFactor;
    void initializeGL() override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    QPixmap pixmap;
    int cellSize;
    QVector<QPoint> points;
    QVector<QPair<QPoint, QPoint>> lines;
    QPoint startPoint;
    bool startPointSet = false;
    DrawMode drawMode = DrawMode::Point;
    static const int WIDTH = 1200;
    static const int HEIGHT = 720;
    QPoint getCoordinates(double x1, double y1, double x2, double y2);
};


#endif //OPTOCUS_GRAPHICSPANEL_H
