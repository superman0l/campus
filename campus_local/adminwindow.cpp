#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "QCloseEvent"
//#include "person.h"//多重定义问题还没解决
#include <QMessageBox>

AdminWindow::AdminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("管理端");
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::closeEvent(QCloseEvent *event)
{
    //重载关闭本窗口的事件
    //定义一个警告对话框
    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,tr("退出管理员界面"),QString(tr("确认退出管理员界面？这将退出整个程序！")),QMessageBox::Yes|QMessageBox::No);
    if(button == QMessageBox::Yes){
        emit ExitAdm();
        event->accept();
    }
    else if(button == QMessageBox::No){
        event->ignore();
    }
}

void AdminWindow::ReceiveAdmData(const Admin * data)
{
    adm = data;//获取管理员信息数据
    this->show();//展示本页面
}

void AdminWindow::on_actionLogOut_O_triggered()
{
    emit LogOut();
}

