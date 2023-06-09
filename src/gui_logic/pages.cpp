//
// Created by Lilly on 26.05.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Pages.h" resolved

#include <QString>
#include "pages.h"
#include "ui_Pages.h"
#include "../pages/HomePageWidget.h"
#include "../pages/ModelingPageWidget.h"
#include "../pages/FormulasPageWidget.h"
#include "../pages/ManualPageWidget.h"


Pages::Pages(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::Pages) {
    ui->setupUi(this);

    // pages
    homePage = new HomePageWidget;
    modelingPage = new ModelingPageWidget;
    calculatorPage = new FormulasPageWidget;
    devicesPage = new ManualPageWidget(ManualPageWidget::ManualType::DevicesPage);
    opticalPhenomenaPage = new ManualPageWidget(ManualPageWidget::ManualType::PhenomPage);

    stackedWidget = ui->stackedWidget;
    stackedWidget->addWidget(homePage);
    stackedWidget->addWidget(modelingPage);
    stackedWidget->addWidget(calculatorPage);
    stackedWidget->addWidget(devicesPage);
    stackedWidget->addWidget(opticalPhenomenaPage);

    // menu bar
    QMenuBar* menuBar = ui->menubar;

    // color and style
    menuBar->setStyleSheet("QMenuBar { background-color: #1D3557; color: #F1FAEE; font-size: 20px; }"
                           "QMenuBar::item:selected { background-color: #457B9D; }"
                           "QMenuBar::item:hover { background-color: #457B9D; }");

    homeAction = new QAction("Головна", this);
    modelingAction = new QAction("Моделювання", this);
    calculatorAction = new QAction("Формули", this);
    devicesAction = new QAction("Пристрої", this);
    opticalPhenomenaAction = new QAction("Оптичні явища", this);

    menuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    menuBar->addAction(homeAction);
    menuBar->addAction(modelingAction);
    menuBar->addAction(calculatorAction);
    menuBar->addAction(devicesAction);
    menuBar->addAction(opticalPhenomenaAction);

    // triggers
    connect(homeAction, &QAction::triggered, [this]() { stackedWidget->setCurrentWidget(homePage); });
    connect(modelingAction, &QAction::triggered, [this]() { stackedWidget->setCurrentWidget(modelingPage); });
    connect(calculatorAction, &QAction::triggered, [this]() { stackedWidget->setCurrentWidget(calculatorPage); });
    connect(devicesAction, &QAction::triggered, [this]() { stackedWidget->setCurrentWidget(devicesPage); });
    connect(opticalPhenomenaAction, &QAction::triggered, [this]() { stackedWidget->setCurrentWidget(opticalPhenomenaPage); });
}

Pages::~Pages() {
    delete ui;
}