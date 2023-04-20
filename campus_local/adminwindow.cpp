#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "QCloseEvent"
#include "person.h"//多重定义问题还没解决
//已添加person.cpp
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
    //if(this->hasFocus()){
        QMessageBox::StandardButton button;
        button=QMessageBox::question(this,tr("退出管理员界面"),QString(tr("确认退出管理员界面？这将退出整个程序！")),QMessageBox::Yes|QMessageBox::No);
        if(button == QMessageBox::Yes){
            qApp->quit();
        }
        else if(button == QMessageBox::No){
            event->ignore();
        }
    //}
    //else{
    //    this->setFocus();
    //    event->ignore();
    //}
}

void AdminWindow::ReceiveAdmData(const Admin * data)
{
    adm = data;//获取管理员信息数据
    this->show();//展示本页面
}

void AdminWindow::on_actionLogOut_O_triggered()
{
    emit LogOut();
    this->hide();
}


void AdminWindow::on_set_clicked()
{
    setcourse* setwind = new setcourse(this);
    setwind->setWindowTitle("修改课程");
    setwind->show();
    setwind->setAttribute(Qt::WA_DeleteOnClose);
}


void AdminWindow::on_add_clicked()
{
    addcourse* addwind = new addcourse(this);
    addwind->setWindowTitle("新增课程");
    addwind->show();
    addwind->setAttribute(Qt::WA_DeleteOnClose);
}


void AdminWindow::on_delete_2_clicked()
{
    QString coursename="xxx";
    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,tr("删除课程"),"确认删除课程：\n"+coursename,QMessageBox::Yes|QMessageBox::No);
    if(button == QMessageBox::Yes){

    }
    else if(button == QMessageBox::No){

    }
}


void AdminWindow::on_set_quiz_clicked()
{
    addexam* addwind = new addexam(this);
    addwind->setWindowTitle("新增课程");
    addwind->show();
    addwind->setAttribute(Qt::WA_DeleteOnClose);
}

