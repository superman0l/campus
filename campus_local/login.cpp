#include "login.h"
#include "ui_login.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QRegularExpressionValidator>
#include "regste.h"

LogIn::LogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    ui->setupUi(this);
    logalready = false;
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

void LogIn::LogConfirm(bool flag)
{
    logalready = flag;
}

void LogIn::closeEvent(QCloseEvent *event)
{
    //重载关闭本窗口的事件
    if(logalready){
        event->accept();
        return ;
    }
    //定义一个警告对话框
    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,tr("退出程序"),QString(tr("确认退出程序")),QMessageBox::Yes|QMessageBox::No);
    if(button == QMessageBox::Yes){
        emit ExitLog();
        event->accept();
    }
    else if(button == QMessageBox::No){
        event->ignore();
    }
}


void LogIn::on_LogIn_2_clicked()
{
    emit SendUser(ui->UserId->text().trimmed(), ui->PassWord->text());//传输内容到主窗口，其中用户名忽略了前置空格
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
    reg->show();
}

void LogIn::Receive_RegData(QString id, QString pswd, QString name, QString clas)
{
    emit SendRegs(id, pswd, name, clas);//向主窗口发送信息
}

void LogIn::ToIdInput()
{
    ui->UserId->setFocus();
}

