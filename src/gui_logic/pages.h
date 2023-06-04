//
// Created by Lilly on 26.05.2023.
//

#ifndef OPTOCUS_PAGES_H
#define OPTOCUS_PAGES_H

#include <QMainWindow>
#include <QStackedWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Pages; }
QT_END_NAMESPACE

class Pages : public QMainWindow {
Q_OBJECT

public:
    explicit Pages(QWidget *parent = nullptr);

    ~Pages() override;

private:
    Ui::Pages *ui;

    QAction* homeAction;
    QAction* modelingAction;
    QAction* calculatorAction;
    QAction* devicesAction;
    QAction* opticalPhenomenaAction;

    QWidget* homePage;
    QWidget* modelingPage;
    QWidget* calculatorPage;
    QWidget* devicesPage;
    QWidget* opticalPhenomenaPage;

    QStackedWidget* stackedWidget;
};


#endif //OPTOCUS_PAGES_H
