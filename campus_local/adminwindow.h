#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

//前置定义一个类
class Admin;
\
namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private:
    Ui::AdminWindow *ui;
    void closeEvent(QCloseEvent *event);
    const Admin* adm;//存放指向管理员类的指针
private slots:
    void ReceiveAdmData(const Admin*);
    void on_actionLogOut_O_triggered();

signals:
    void ExitAdm();//关闭本窗口
    void LogOut();//退出登录信号
};

#endif // ADMINWINDOW_H
