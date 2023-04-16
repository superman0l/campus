#include "login.h"
#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MainWindow w;//为了设计主页面而直接运行主页面
   // w.show();
    LogIn l;
    l.show();//进入登录界面
    return a.exec();
}
