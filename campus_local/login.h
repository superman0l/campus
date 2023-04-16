#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
\
namespace Ui {
class LogIn;
}

//前置定义
class Regste;
class MainWindow;
class AdminWindow;
class User;
class Admin;

class LogIn : public QDialog
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = nullptr);
    ~LogIn();

protected:
    void closeEvent(QCloseEvent *event);//处理关闭窗口事件
private slots:

    void on_LogIn_2_clicked();

    void on_Regester_clicked();

    void Receive_RegData(QString, QString, QString, QString);//接受注册窗口传来的信息

    void ToIdInput();//从注册窗口返回时，定位光标

private:
    Ui::LogIn *ui;
    Regste* reg;//保存一个注册窗口的指针
    MainWindow* useWin;//用户的主功能窗口
    AdminWindow* admWin;//管理员的功能窗口
signals:
    //void SendUser(const User*);//发送普通用户信息
    //void SendAdmn(const Admin*);//发送管理员信息
};

#endif // LOGIN_H
