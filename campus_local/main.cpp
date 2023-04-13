#include "login.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogIn l;
    l.show();//进入登录界面
    //w.show();
    return a.exec();
}
