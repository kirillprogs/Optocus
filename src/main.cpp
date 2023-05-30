#pragma comment (lib, "opengl32.lib")
#include <QApplication>
#include <QIcon>
#include <GL\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include "style/OptStyle.h"
#include "pages/gui_logic/pages.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");
    QApplication::setPalette(QApplication::style()->standardPalette());

    OptStyle myStyle;
    QApplication::setStyle(&myStyle);

    QIcon icon("../images/logo.png");
    QApplication::setWindowIcon(icon);

    Pages pages;
    pages.showMaximized();
    return QApplication::exec();
}