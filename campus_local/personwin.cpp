#include "personwin.h"
#include "ui_personwin.h"

PersonWin::PersonWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonWin)
{

    ui->setupUi(this);
}

PersonWin::~PersonWin()
{
    delete ui;
}
