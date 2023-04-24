#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"person.h"

//前置定义
class timer;
class QLabel;

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
    //const User* per;//存放登录的用户
    void closeEvent(QCloseEvent *event);//处理关闭窗口事件
    //timer* tim;//保存时间
    QLabel* timlabl;//显示时间的标签

private slots:
    void on_action_S_triggered();//切换用户的槽

    void timShowNewest();//用于更新时间信息的槽
    void on_actionBeginTim_B_triggered();

    void on_actionPauseTim_P_triggered();

    void on_actionChgTimRa_T_triggered();

signals:
    void LogOut();
    //void UserOut(const User*);
};
#endif // MAINWINDOW_H
