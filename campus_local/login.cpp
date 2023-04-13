#include "login.h"
#include "ui_login.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QRegularExpressionValidator>
#include "person.h"
#include "mainwindow.h"
#include "adminwindow.h"
#include "regste.h"

LogIn::LogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    ui->setupUi(this);
    //初始化指针
    admWin = nullptr;
    useWin = nullptr;
    //设置密码回显为黑点
    ui->PassWord->setEchoMode(QLineEdit::Password);

    //设置输入框内的显示内容
    ui->UserId->setPlaceholderText("请输入用户ID");
    ui->PassWord->setPlaceholderText("请输入密码");

    //设置一键清除按钮
    ui->UserId->setClearButtonEnabled(true);

    //设置输入框标题
    this->setWindowTitle("登录窗口");

    //限定用户输入内容
    QRegularExpressionValidator * idRegVal = new QRegularExpressionValidator(this);
    QRegularExpressionValidator * pswdRegVal = new QRegularExpressionValidator(this);
    //通过RegularExpression规范内容
    //将用户ID设置为最多(或者正则表达式中必须)10位的非负整数
    idRegVal->setRegularExpression(QRegularExpression(tr("^\\d{10}")));
    ui->UserId->setValidator(idRegVal);
    //将密码设置为（正则表达式中至少六位的）由数字、大小写字母、下划线组成的字符串
    pswdRegVal->setRegularExpression(QRegularExpression(tr("^\\w{6,}")));
    ui->PassWord->setValidator(pswdRegVal);

    //美化输入框

}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::closeEvent(QCloseEvent *event)
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


void LogIn::on_LogIn_2_clicked()
{
    //emit SendUser(ui->UserId->text().trimmed(), ui->PassWord->text());//传输内容到主窗口，其中用户名忽略了前置空格
    if(ui->UserId->text().isEmpty()||ui->PassWord->text().isEmpty()){
        //处理为空时的情况
        QMessageBox::warning(this, tr("警告！"), tr("用户ID或密码不能为空！"), QMessageBox::Ok);
    }
    else if(login(ui->UserId->text(), ui->PassWord->text(), userptr, admnptr)){
        //logdlg->LogConfirm(true);//改变登录状态，避免弹出警告框
        this->hide();//隐藏登录窗口
        if(admnptr){
            //是管理员用户，进入管理界面
            //展示登录成功信息
            QMessageBox::information(this, tr("成功！"), tr("欢迎您，尊敬的管理员！"), QMessageBox::Ok);
            //新建一个管理员界面
            if(admWin != nullptr)
                delete admWin;
            admWin = new AdminWindow(this);
            //实现数据传输
            connect(this, SIGNAL(SendAdmn(const Admin*)), admWin, SLOT(ReceiveAdmData(const Admin*)));
            //实现切换用户
            connect(admWin, SIGNAL(LogOut()), this, SLOT(ToIdInput()));
            //进入管理员界面
            admWin->show();
        }
        else if(userptr){
            //是一般用户，进入主窗口
            //展示登录成功信息
            QMessageBox::information(this, tr("成功！"), tr("登录成功，正在跳转"), QMessageBox::Ok);
            //新建学生功能窗口界面
            if(useWin != nullptr)
                delete useWin;
            useWin = new MainWindow(this);
            //实现数据传输
            connect(this, SIGNAL(SendUser(const User*)), useWin, SLOT(ReiceiveUser(const User*)));
            //实现切换用户
            connect(useWin, SIGNAL(LogOut()), this, SLOT(ToIdInput()));
            //进入学生界面
            useWin->show();
        }
        else{
            //函数执行出错
            qDebug() << "Fatal error!";
        }
    }
    else{
        //登录失败，弹出一个警告框
        QMessageBox::warning(this, tr("失败！"), tr("用户ID不存在或密码错误，请重试！"), QMessageBox::Ok);
        qDebug() << "登陆失败";
    }
    ui->UserId->clear();ui->PassWord->clear();//清理输入框内的内容
    ui->UserId->setFocus();//定位光标
}

void LogIn::on_Regester_clicked()
{
    //建立注册窗口
    reg = new Regste(this);
    //连接本窗口与注册窗口
    connect(reg, SIGNAL(SendRegData(QString,QString,QString,QString)), this, SLOT(Receive_RegData(QString,QString,QString,QString)));
    //实现从注册窗口返回时光标的位置定位
    connect(reg, SIGNAL(RetToLogIn()), this, SLOT(ToIdInput()));
    //展示注册窗口
    this->hide();
    reg->show();
}

void LogIn::Receive_RegData(QString id, QString pswd, QString name, QString clas)
{
    //emit SendRegs(id, pswd, name, clas);//向主窗口发送信息
    if(!sign_up(id, pswd, name, clas)){
        //弹出警告框
        QMessageBox::information(this, tr("注册失败"), tr("注册失败，请重试"), QMessageBox::Ok);
        qDebug() << "注册失败";
    }
    else{
        //注册成功，登录后自动进入主窗口
        //先弹出一个告知窗口
        QMessageBox::information(this, tr("注册成功"), tr("正在为您自动登录"), QMessageBox::Ok);
        delete reg;
        ui->UserId->setText(id);ui->PassWord->setText(pswd);
        on_LogIn_2_clicked();
    }
}

void LogIn::ToIdInput()
{
    this->show();
    ui->UserId->setFocus();
}

