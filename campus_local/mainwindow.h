#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"person.h"


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
    const User* per;//存放登录的用户
    void closeEvent(QCloseEvent *event);//处理关闭窗口事件
private slots:
    void on_action_S_triggered();//切换用户的槽
    void ReiceiveUser(const User*);
signals:
    void LogOut();
};
#endif // MAINWINDOW_H
