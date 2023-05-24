//
// Created by kirill on 24/05/23.
//
#include <QApplication>
#include <QPushButton>

#include "Header.h"

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    function();
    return QApplication::exec();
}