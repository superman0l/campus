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
    LogIn l;
    l.show();//进入登录界面
    qss.close();
    return a.exec();
}
