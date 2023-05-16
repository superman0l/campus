#include "login.h"
#include "mainwindow.h"
#include <QApplication>
#include "online_data.h"
#include "map.h"
/*#include <windef.h>
#include <wingdi.h>
#include <winuser.h>*/

int main(int argc, char *argv[])
{
    /*DEVMODE NewDevMode;//获取屏幕设置中的分辨率
    EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &NewDevMode);
    qreal  cx = NewDevMode.dmPelsWidth;			//当前设置的屏幕宽度
    qreal scale = cx / 2240;
    qputenv("QT_SCALE_FACTOR", QString::number(scale).toLatin1());*/


    QApplication::setAttribute(Qt::AA_Use96Dpi);
    QApplication::setQuitOnLastWindowClosed(false);
    school_online=new map("../Json/map.json");
    if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)){
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Unset);
    }
    QApplication a(argc, argv);
    QFile qss("../StyleSheet.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    //MainWindow w;//为了设计主页面而直接运行主页面
   // w.show();
    LogIn l;
    l.show();//进入登录界面
    return a.exec();
    qss.close();
}
