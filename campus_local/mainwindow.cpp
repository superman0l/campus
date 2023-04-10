#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "person.h"
#include "login.h"
#include "adminwindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QCloseEvent>

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
    //实现尚未登录成功时，登录窗口关闭后此窗口紧接着关闭
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::ReceiveLoginData(QString userid, QString pass)
{
    if(userid.isEmpty()||pass.isEmpty()){
        //处理为空时的情况
        QMessageBox::warning(logdlg, tr("警告！"), tr("用户ID或密码不能为空！"), QMessageBox::Ok);
    }
    else if(login(userid, pass, per, adm)){
        logdlg->LogConfirm(true);//改变登录状态，避免弹出警告框
        logdlg->hide();//隐藏登录窗口
        if(adm){
            //是管理员用户，进入管理界面
            //展示登录成功信息
            QMessageBox::information(logdlg, tr("成功！"), tr("欢迎您，尊敬的管理员！"), QMessageBox::Ok);
            //新建一个管理员界面
            AdmW = new AdminWindow(this);
            //连接此界面与管理员界面
            connect(this, SIGNAL(SendAdmPtr(const Admin*)), AdmW, SLOT(ReceiveAdmData(const Admin*)));
            //实现管理员界面退出时，此界面也退出
            connect(AdmW, SIGNAL(ExitAdm()), this, SLOT(close()));
            //实现管理员退出登录
            connect(AdmW, SIGNAL(LogOut()), this, SLOT(LogOutFromAdmin()));
            //向管理员界面传输信息
            emit SendAdmPtr(adm);
        }
        else if(per){
            //是一般用户，进入主窗口
            //展示登录成功信息
            QMessageBox::information(logdlg, tr("成功！"), tr("登录成功，正在跳转"), QMessageBox::Ok);
            this->setWindowTitle("学生端");
            this->show();//打开主窗口
        }
        else{
            //函数执行出错
            qDebug() << "Fautal error!";
        }
    }
    else{
        //登录失败，弹出一个警告框
        QMessageBox::warning(logdlg, tr("失败！"), tr("用户ID不存在或密码错误，请重试！"), QMessageBox::Ok);
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
    //未登录时登录窗口关闭，紧随着关闭此窗口
    //管理员窗口关闭时，紧随着关闭此窗口
    //this->show();
    qApp->quit();
}

void MainWindow::on_action_S_triggered()
{
    //实现学生端切换用户的功能
    this->hide();//隐藏主窗口
    delete per;//删除原登录数据
    logdlg->LogConfirm(false);//重设登录状态
    logdlg->show();//打开登录界面
}

void MainWindow::LogOutFromAdmin()
{
    //实现管理员端切换用户的功能
    delete AdmW;//删除管理员窗口
    delete adm;//删除管理员数据
    logdlg->LogConfirm(false);//重设登录状态
    logdlg->show();//打开登录界面
}
