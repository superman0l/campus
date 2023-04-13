#include "classeswind.h"
#include "ui_classeswind.h"

ClassesWind::ClassesWind(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ClassesWind)
{
    ui->setupUi(this);
    //清除标题
    QWidget *oldTitalbar = this->titleBarWidget();
    QWidget *newTitalbar = new QWidget();
    this->setTitleBarWidget(newTitalbar);
    delete oldTitalbar;
}

ClassesWind::~ClassesWind()
{
    delete ui;
}
