//
// Created by Lilly on 26.05.2023.
//

#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QSizePolicy>
#include <QScrollArea>
#include "ModelingPageWidget.h"
#include "../style/OptStyle.h"

ModelingPageWidget::ModelingPageWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *layout = new QHBoxLayout;

    GraphicsPanel* drawingWidget = new GraphicsPanel(this);
    layout->addWidget(drawingWidget, 4);

    QWidget *rightPanel = new QWidget;
    QVBoxLayout *layout2 = new QVBoxLayout(rightPanel);

    // Верхня права панель з кнопками
    QWidget *buttonPanel = new QWidget;
    QVBoxLayout *buttonPanelLayout = new QVBoxLayout(buttonPanel);

    QPushButton *addLensButton = new QPushButton("Додати лінзу");
    connect(addLensButton, &QPushButton::clicked, [=]() {
        drawingWidget->addLens();
    });
    QPushButton *drawRayButton = new QPushButton("Намалювати промінь");
    connect(drawRayButton, &QPushButton::clicked, [=]() {
        drawingWidget->setDrawMode(GraphicsPanel::DrawMode::Ray);
    });
    /*QPushButton *drawObjectButton = new QPushButton("Намалювати тіло");
    connect(drawObjectButton, &QPushButton::clicked, [=]() {
        drawingWidget->setDrawMode(GraphicsPanel::DrawMode::Line);
    });*/
    QPushButton *drawPointButton = new QPushButton("Намалювати об'єкт");
    connect(drawPointButton, &QPushButton::clicked, [=]() {
        drawingWidget->setDrawMode(GraphicsPanel::DrawMode::Point);
    });
    QPushButton *addObjectButton = new QPushButton("Додати об'єкт");
    connect(addObjectButton, &QPushButton::clicked, [=]() {
        drawingWidget->addObject();
    });
    QPushButton *changeScaleButton = new QPushButton("Поміняти масштаб");
    connect(changeScaleButton, &QPushButton::clicked, [=]() {
        drawingWidget->setCellScale();
    });
    QPushButton *saveButton = new QPushButton("Зберегти модель", this);
    connect(saveButton, &QPushButton::clicked, [=]() {
        drawingWidget->saveModel();
    });
    QWidget *clear = new QWidget;
    QHBoxLayout *clearLayout = new QHBoxLayout(clear);
    QPushButton *clearDrawingsButton = new QPushButton("Очистити");
    connect(clearDrawingsButton, &QPushButton::clicked, [=]() {
        drawingWidget->clearGeometry();
    });
    QPushButton *clearButton = new QPushButton("Очистити все");
    connect(clearButton, &QPushButton::clicked, [=]() {
        drawingWidget->clearPanel();
    });
    clearLayout->addWidget(clearDrawingsButton);
    clearLayout->addWidget(clearButton);
    clear->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

    buttonPanelLayout->addWidget(addLensButton);
    buttonPanelLayout->addWidget(drawRayButton);
    // buttonPanelLayout->addWidget(drawObjectButton);
    buttonPanelLayout->addWidget(drawPointButton);
    buttonPanelLayout->addWidget(addObjectButton);
    buttonPanelLayout->addWidget(changeScaleButton);
    buttonPanelLayout->addWidget(saveButton);
    buttonPanelLayout->addWidget(clear);

    // configure results display
    QLabel *resultsLabel = new QLabel;
    resultsLabel->setWordWrap(true);

    QFont font = resultsLabel->font();
    font.setPointSize(12);
    resultsLabel->setFont(font);

    QScrollArea *scrollArea = new QScrollArea;
    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
    scrollLayout->addWidget(resultsLabel);

    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);

    connect(drawingWidget, &GraphicsPanel::calculationsUpdated, resultsLabel, &QLabel::setText);
    QPalette pal;
    scrollArea->setWidgetResizable(true);
    pal.setColor(QPalette::Window, OptStyle::MINT_GREEN);
    scrollWidget->setAutoFillBackground(true);
    scrollWidget->setPalette(pal);

    layout2->addWidget(buttonPanel, 1);
    layout2->addWidget(scrollArea, 1);
    layout->addWidget(rightPanel, 1);


    mainLayout->addLayout(layout);
    setLayout(mainLayout);
}
