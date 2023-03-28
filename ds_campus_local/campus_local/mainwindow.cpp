#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "person.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Register_clicked()
{
    if(!sign_up(ui->rgs_id->toPlainText(),ui->rgs_pswd->toPlainText(),ui->rgs_name->toPlainText(),ui->rgs_class->toPlainText())){
        qDebug()<<"sign failed.";
    }
}


void MainWindow::on_login_clicked()
{
    const User* stu=login(ui->login_id->toPlainText(),ui->login_pswd->toPlainText());
    qDebug()<<QString::fromStdString(stu->get_name());
}

