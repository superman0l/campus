#include "mainwindow.h"
#include <QApplication>
#include "map.h"
int main(int argc, char *argv[])
{
    //map benbu_map = map("map.json");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
