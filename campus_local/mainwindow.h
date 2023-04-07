#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"
class User;

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
    const User* per;//存放正在登录的用户
private slots:
    void ReceiveLoginData(QString, QString);//接受登录信息
    void ReceiveRegstData(QString, QString, QString, QString);//接受注册信息
    void close();//关闭主窗口
};
#endif // MAINWINDOW_H
