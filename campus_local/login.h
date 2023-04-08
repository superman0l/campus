#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
\
namespace Ui {
class LogIn;
}

class Regste;//前置定义

class LogIn : public QDialog
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = nullptr);
    ~LogIn();
    void LogConfirm(bool flag);//确认登录状态
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
    bool logalready;//保存是否已经登录的信息
signals:
    void SendUser(QString user, QString password);//发送账号密码
    void SendRegs(QString id, QString pswd, QString name, QString clas);//发送注册信息
    void ExitLog();
};

#endif // LOGIN_H
