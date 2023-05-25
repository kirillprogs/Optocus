#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QGraphicsScene * scene = new QGraphicsScene;
    QGraphicsRectItem * rect = new QGraphicsRectItem;
    rect->setRect(0, 0, 50, 40);
    scene->addItem(rect);
    QGraphicsView * view = new QGraphicsView(scene);
    view->show();
    return QApplication::exec();
}