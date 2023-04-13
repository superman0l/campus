#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //在状态栏显示信息
    QLabel *perm = new QLabel(this);
    perm->setFrameStyle(QFrame::Box|QFrame::Sunken);
    perm->setText(tr("<a href=\"https://live.bilibili.com/22603245?live_from=85001&spm_id_from=444.41.live_users.item.click\">项目源码地址<a>"));
    perm->setTextFormat(Qt::RichText);
    perm->setOpenExternalLinks(true);
    ui->statusbar->addPermanentWidget(perm);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //重载关闭本窗口的事件
    //定义一个警告对话框
    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,tr("退出程序"),QString(tr("确认退出程序")),QMessageBox::Yes|QMessageBox::No);
    if(button == QMessageBox::Yes){
        qApp->quit();
    }
    else if(button == QMessageBox::No){
        event->ignore();
    }
}

void MainWindow::on_action_S_triggered()
{
    //实现学生端切换用户的功能
    emit LogOut();
    this->hide();
}

void MainWindow::ReiceiveUser(const User * data)
{
    per = data;
}
