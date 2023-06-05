#pragma comment (lib, "opengl32.lib")
#include <QApplication>
#include <QIcon>
#include "style/OptStyle.h"
#include "gui_logic/pages.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    QApplication::setStyle(new OptStyle);

    QIcon icon("../images/logo.png");
    QApplication::setWindowIcon(icon);

    Pages pages;
    pages.showMaximized();
    return QApplication::exec();
}