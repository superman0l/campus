#include "regste.h"
#include "ui_regste.h"
#include <QMessageBox>

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
    ui->name->setPlaceholderText("请输入账号名称");ui->clas->setPlaceholderText("请输入班级信息");
}

Regste::~Regste()
{
    delete ui;
}

void Regste::on_RegesConfirm_clicked()
{
    if(ui->pswd->text() != ui->Repswd->text()){
        QMessageBox::warning(this, tr("警告"), tr("两次输入的密码不一致，请再次设置密码"), QMessageBox::Ok);
        ui->pswd->clear();
        ui->Repswd->clear();
        ui->pswd->setFocus();
    }
    else{
        emit SendRegData(ui->id->text(), ui->pswd->text(), ui->name->text().trimmed(), ui->clas->text().trimmed());//传输数据
        this->close();
    }
}

