#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "setcourse.h"
#include "addcourse.h"
#include "addexam.h"
#include <QStandardItemModel>

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
    QStandardItemModel* model = new QStandardItemModel(13, 7);
    void load(int weeknum);

private:
    Ui::AdminWindow *ui;
    void closeEvent(QCloseEvent *event);
    const Admin* adm;//存放指向管理员类的指针
private slots:
    void ReceiveAdmData(const Admin*);
    void on_actionLogOut_O_triggered();

    void on_set_clicked();

    void on_add_clicked();

    void on_delete_2_clicked();

    void on_set_quiz_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void fresh();

signals:
    void LogOut();//退出登录信号
    void sendsetData(QModelIndex indx, QString show);
};

#endif // ADMINWINDOW_H
