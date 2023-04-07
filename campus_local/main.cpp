#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show(); //主函数里不打开mainwindow窗口，而是通过槽来打开
    return a.exec();
}
