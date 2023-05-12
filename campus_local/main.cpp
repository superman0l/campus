#include "login.h"
#include "mainwindow.h"
#include <QApplication>
#include "online_data.h"
#include "map.h"
int main(int argc, char *argv[])
{
    QApplication::setQuitOnLastWindowClosed(false);
    school_online=new map("../Json/map.json");
    if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Round);
    QApplication a(argc, argv);

    //MainWindow w;//为了设计主页面而直接运行主页面
   // w.show();
    LogIn l;
    l.show();//进入登录界面
    return a.exec();
}
