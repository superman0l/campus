#include "addexam.h"
#include "ui_addexam.h"

addexam::addexam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addexam)
{
    ui->setupUi(this);
}

addexam::~addexam()
{
    delete ui;
}
