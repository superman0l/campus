#include "regste.h"
#include "ui_regste.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QCloseEvent>

Regste::Regste(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Regste)
{
    ui->setupUi(this);
    //定义输入框的一些信息
    this->setWindowTitle("注册窗口");
    ui->id->setPlaceholderText("请输入账号id");
    ui->pswd->setPlaceholderText("请输入密码");ui->pswd->setEchoMode(QLineEdit::Password);
    ui->Repswd->setPlaceholderText("请再次输入密码");ui->Repswd->setEchoMode(QLineEdit::Password);
    ui->name->setPlaceholderText("请输入账号名称");
    ui->clas->setPlaceholderText("请输入班级信息");

    //为输入框添加限制
    QRegularExpressionValidator* NumRegVal = new QRegularExpressionValidator(this);
    QRegularExpressionValidator* PswdRegVal = new QRegularExpressionValidator(this);
    //QRegularExpressionValidator* NameRegVal = new QRegularExpressionValidator(this);
    //设置id与班级为最多10位的非负整数/纯数字
    NumRegVal->setRegularExpression(QRegularExpression(tr("^\\d{10}")));
    ui->id->setValidator(NumRegVal);ui->clas->setValidator(NumRegVal);
    //设置密码为至少6位的由数字、大小写字母、下划线组成的字符串
    PswdRegVal->setRegularExpression(QRegularExpression(tr("^\\w{6,}")));
    ui->pswd->setValidator(PswdRegVal);ui->Repswd->setValidator(PswdRegVal);
    //设置名称为最多18位的任意形式字符串
    //NameRegVal->setRegularExpression(QRegularExpression(tr("\\w{18}")));
    ui->name->setMaxLength(18);

    //定义光标位置
    //ui->id->setFocus();
}

Regste::~Regste()
{
    delete ui;
}

void Regste::closeEvent(QCloseEvent *event)
{
    emit RetToLogIn();
    event->accept();
}

void Regste::on_RegesConfirm_clicked()
{
    if(ui->id->text().isEmpty()){
        //id栏留空
        QMessageBox::warning(this, tr("警告"), tr("用户ID不能为空"), QMessageBox::Ok);
        ui->id->setFocus(Qt::OtherFocusReason);
    }
    else if(ui->pswd->text().isEmpty()||ui->Repswd->text().isEmpty()){
        //密码栏留空
        QMessageBox::warning(this, tr("警告"), tr("密码栏不能为空"), QMessageBox::Ok);
        if(ui->pswd->text().isEmpty())
            ui->pswd->setFocus(Qt::OtherFocusReason);
        else
            ui->Repswd->setFocus();
    }
    else if(ui->name->text().isEmpty()){
        //名称栏留空
        QMessageBox::warning(this, tr("警告"), tr("名称栏不能为空"), QMessageBox::Ok);
        ui->name->setFocus();
    }
    else if(ui->clas->text().isEmpty()){
        //班级栏留空
        QMessageBox::warning(this, tr("警告"), tr("班级栏不能为空"), QMessageBox::Ok);
        ui->clas->setFocus();
    }
    else if(ui->pswd->text() != ui->Repswd->text()){
        //两次输入密码不一致
        QMessageBox::warning(this, tr("警告"), tr("两次输入的密码不一致，请再次设置密码"), QMessageBox::Ok);
        ui->pswd->clear();
        ui->Repswd->clear();
        ui->pswd->setFocus();
    }
    else{
        emit SendRegData(ui->id->text(), ui->pswd->text(), ui->name->text(), ui->clas->text().trimmed());//传输数据
        this->close();
    }
}

void Regste::on_RetToLogIn_clicked()
{
    //返回登录窗口
    this->close();
}

