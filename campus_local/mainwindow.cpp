#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "person.h"
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    logdlg = new LogIn(this);
    //展示登录界面
    logdlg->show();
    //连接信号与槽，实现信息传输
    connect(logdlg, SIGNAL(SendUser(QString,QString)), this, SLOT(ReceiveLoginData(QString,QString)));
    connect(logdlg, SIGNAL(SendRegs(QString,QString,QString,QString)), this, SLOT(ReceiveRegstData(QString,QString,QString,QString)));
    //实现尚未登录成功时，登录窗口关闭此窗口直接关闭
    connect(logdlg, SIGNAL(ExitLog()), this, SLOT(close()));
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

void MainWindow::ReceiveLoginData(QString userid, QString pass)
{
    per = login(userid, pass);
    if(per != NULL){
        QMessageBox::information(logdlg, tr("成功！"), tr("登录成功，正在跳转"), QMessageBox::Ok);
        logdlg->LogConfirm(true);
        logdlg->close();//关闭登录窗口
        this->show();//打开主窗口
    }
    else{
        //弹出一个警告框
        QMessageBox::warning(logdlg, tr("失败！"), tr("登录失败，请重试"), QMessageBox::Ok);
        qDebug() << "登陆失败";
    }
}

void MainWindow::ReceiveRegstData(QString id, QString pswd, QString name, QString clas)
{
    if(!sign_up(id, pswd, name, clas)){
        //弹出警告框
        QMessageBox::information(logdlg, tr("注册失败"), tr("注册失败，请重试"), QMessageBox::Ok);
        qDebug() << "注册失败";
    }
    else{
        //注册成功，登录后自动进入主窗口
        //先弹出一个告知窗口
        QMessageBox::information(logdlg, tr("注册成功"), tr("正在为您自动登录"), QMessageBox::Ok);
        ReceiveLoginData(id, pswd);
    }
}

void MainWindow::close()
{
    //登录窗口关闭，紧随着关闭此窗口
    this->close();
}
