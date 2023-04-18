#include "mapwin.h"
#include "ui_mapwin.h"

MapWin::MapWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapWin)
{

    ui->setupUi(this);
}

MapWin::~MapWin()
{
    delete ui;
}
