#include "setcourse.h"
#include "ui_setcourse.h"

setcourse::setcourse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcourse)
{
    ui->setupUi(this);
}

setcourse::~setcourse()
{
    delete ui;
}

void setcourse::closeEvent(QCloseEvent *event)
{

}
