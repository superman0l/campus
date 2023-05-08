#include "classwindow.h"
#include "ui_classwindow.h"
#include <QHBoxLayout>


ClassWindow::ClassWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassWindow)
{
    ui->setupUi(this);
}

ClassWindow::~ClassWindow()
{
    delete ui;
}
