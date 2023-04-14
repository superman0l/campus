#include "affairwin.h"
#include "ui_affairwin.h"

AffairWin::AffairWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AffairWin)
{
    ui->setupUi(this);
}

AffairWin::~AffairWin()
{
    delete ui;
}
