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
    if(!sign_up(ui->rgs_id->toPlainText(),ui->rgs_pswd->toPlainText())){
        qDebug()<<"sign failed.";
    }
}

