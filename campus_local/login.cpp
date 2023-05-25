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
#include "basic.h"

extern const User* user_online;
extern const Admin* admin_online;


LogIn::LogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    this->setWindowIcon(QIcon("../icon.png"));
    this->setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint);
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
    if(ui->UserId->text().isEmpty()||ui->PassWord->text().isEmpty()){
        //处理为空时的情况
        QMessageBox::warning(this, tr("警告！"), tr("用户ID或密码不能为空！"), QMessageBox::Ok);
        return;
    }
    int flag=login(ui->UserId->text(), ui->PassWord->text());
    if(flag==1){
        //logdlg->LogConfirm(true);//改变登录状态，避免弹出警告框
        this->hide();//隐藏登录窗口
        if(admin_online){
            //是管理员用户，进入管理界面
            //展示登录成功信息
            QMessageBox::information(this, tr("成功！"), tr("欢迎您，尊敬的管理员！"), QMessageBox::Ok);
            //新建一个管理员界面
            if(admWin != nullptr)
                delete admWin;
            admWin = new AdminWindow(this);
            //实现数据传输        
            //connect(this, SIGNAL(SendAdmn(const Admin*)), admWin, SLOT(ReceiveAdmData(const Admin*)));
            //实现切换用户
            connect(admWin, SIGNAL(LogOut()), this, SLOT(ToIdInput()));
            //进入管理员界面
            admWin->show();
        }
        else if(user_online){
            //是一般用户，进入主窗口
            //展示登录成功信息
            QMessageBox::information(this, tr("成功！"), tr("登录成功，正在跳转"), QMessageBox::Ok);
            //新建学生功能窗口界面
            if(useWin != nullptr)
                delete useWin;
            useWin = new MainWindow(this);
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
    else if(flag==0){
        //登录失败，弹出一个警告框
        QMessageBox::warning(this, tr("失败！"), tr("用户ID不存在或密码错误，请重试！"), QMessageBox::Ok);
        qDebug() << "登陆失败";
    }
    else if(flag==-1)
        QMessageBox::warning(this, tr("失败！"), tr("当前管理员正在维护教务系统，请稍后再试。"), QMessageBox::Ok);
    else if(flag==-3)
        QMessageBox::warning(this, tr("失败！"), tr("打开文件失败，请联系管理员更新信息。"), QMessageBox::Ok);
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
    int flag=sign_up(id, pswd, name, clas);
    if(flag==-2){
        //弹出警告框
        QMessageBox::information(this, tr("注册失败"), tr("该用户已经注册，请更换用户"), QMessageBox::Ok);
        qDebug() << "注册失败,用户已被注册";
    }
    else if(flag==-1){
        QMessageBox::information(this, tr("注册失败"), tr("该班级尚不存在，请联系管理员"), QMessageBox::Ok);
        qDebug() << "注册失败,该班级不存在";
    }
    else if(flag==0){
        QMessageBox::information(this, tr("注册失败"), tr("文件读写失败，请联系管理员"), QMessageBox::Ok);
        qDebug() << "注册失败,文件读取失败";
    }
    else if(flag==1){
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
    if(useWin)
    {
        //退出登录时，存储用户placeid信息,建议改到User的析构函数里
        QJsonObject obj;
        open_json(QString::number(user_online->get_id()) + ".json", obj);
        obj["place_id"] = user_online->get_place_id();
        write_json(QString::number(user_online->get_id()) + ".json", obj);
        qDebug() << "Saved";
    }
    this->show();
    ui->UserId->setFocus();
}
