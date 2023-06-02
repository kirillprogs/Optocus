//
// Created by Lilly on 28.05.2023.
//

#ifndef OPTOCUS_GRAPHICS_PANEL_H
#define OPTOCUS_GRAPHICS_PANEL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QFileDialog>
#include <QInputDialog>
#include "OpticalController.h"

class GraphicsPanel : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    enum class DrawMode {
        Point,
        Line,
        Ray
    };

    void addObject();

private:
    static const int WIDTH = 1200;
    static const int HEIGHT = 720;
    static const int CELL_NUM = 30;
    QPixmap pixmap;
    int cellSize;
    QVector<QPair<QPoint, QPoint>> rays;
    QPoint startPoint;
    bool startPointSet = false;
    DrawMode drawMode = DrawMode::Point;
    OpticalController *controller;
    float scaleFactor;

public:
    explicit GraphicsPanel(QWidget* parent = nullptr);

    void setDrawMode(DrawMode mode);

    void clearPanel();
    void saveModel();
    void addLens();

private:
    int centerX();
    int centerY();
    void draw_cells(QPainter&);
    void draw_axis(QPainter&);
    void draw_object(QPainter&);
    void draw_images(QPainter&);
    void draw_image_rays(QPainter&);
    void draw_rays(QPainter&);
    void draw_lens(const Lens&, QPainter&);
    void initializeGL() override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    QPoint getCoordinates(double x, double y);

};

#endif //OPTOCUS_GRAPHICS_PANEL_H