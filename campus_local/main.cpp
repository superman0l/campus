#include "login.h"
#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    //LogIn l;
    //l.show();//进入登录界面
    //w.show();
    return a.exec();
}
