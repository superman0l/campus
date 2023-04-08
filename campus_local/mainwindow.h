#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//四个类的前置定义
class LogIn;
class AdminWindow;
class User;
class Admin;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LogIn* logdlg;//存放指向登录的子窗口的指针
    AdminWindow* AdmW;//存放指向管理员页面的窗口的指针
    const User* per;//存放正在登录的用户
    const Admin* adm;//存放可能的管理员用户
private slots:
    void ReceiveLoginData(QString, QString);//接受登录信息
    void ReceiveRegstData(QString, QString, QString, QString);//接受注册信息
    void close();//关闭主窗口
    void on_action_S_triggered();//切换用户的槽
    void LogOutFromAdmin();//实现管理员的退出登录
signals:
    void SendAdmPtr(const Admin*);//传输数据到管理员界面
};
#endif // MAINWINDOW_H
