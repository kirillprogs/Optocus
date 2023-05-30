//
// Created by Lilly on 26.05.2023.
//

#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include "ModelingPageWidget.h"

ModelingPageWidget::ModelingPageWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);

    GraphicsPanel* drawingWidget = new GraphicsPanel(this);
    layout->addWidget(drawingWidget, 4);

    QWidget *rightPanel = new QWidget;
    QVBoxLayout *layout2 = new QVBoxLayout(rightPanel);
    layout2->setContentsMargins(0, 0, 0, 0);

    // Верхня права панель з кнопками
    QWidget *buttonPanel = new QWidget;
    QVBoxLayout *buttonPanelLayout = new QVBoxLayout(buttonPanel);
    buttonPanelLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *addLensButton = new QPushButton("Додати лінзу");
    connect(addLensButton, &QPushButton::clicked, [=]() {
        drawingWidget->addLens();
    });
    QPushButton *drawRayButton = new QPushButton("Намалювати промінь");
    QPushButton *drawObjectButton = new QPushButton("Намалювати тіло");
    connect(drawObjectButton, &QPushButton::clicked, [=]() {
        drawingWidget->setDrawMode(GraphicsPanel::DrawMode::Line);
    });
    QPushButton *drawPointButton = new QPushButton("Намалювати точку");
    connect(drawPointButton, &QPushButton::clicked, [=]() {
        drawingWidget->setDrawMode(GraphicsPanel::DrawMode::Point);
    });
    QPushButton *changeScaleButton = new QPushButton("Поміняти масштаб");
    QPushButton *saveButton = new QPushButton("Зберегти модель", this);
    connect(saveButton, &QPushButton::clicked, [=]() {
        drawingWidget->saveModel();
    });
    QPushButton *clearButton = new QPushButton("Очистити");
    connect(clearButton, &QPushButton::clicked, [=]() {
        drawingWidget->clearPanel();
    });

    buttonPanelLayout->addWidget(addLensButton);
    buttonPanelLayout->addWidget(drawRayButton);
    buttonPanelLayout->addWidget(drawObjectButton);
    buttonPanelLayout->addWidget(drawPointButton);
    buttonPanelLayout->addWidget(changeScaleButton);
    buttonPanelLayout->addWidget(saveButton);
    buttonPanelLayout->addWidget(clearButton);

    // Нижня права панель результатів обчислень
    QTextEdit *resultsText = new QTextEdit;

    layout2->addWidget(buttonPanel, 1);
    layout2->addWidget(resultsText, 1);

    layout->addWidget(rightPanel, 1);


    mainLayout->addLayout(layout);
    setLayout(mainLayout);
}
