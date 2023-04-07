#include "mainwindow.h"
#include "map.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //map benbu_map = map("map.json");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
