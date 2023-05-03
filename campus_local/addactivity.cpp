#include "addactivity.h"
#include "ui_addactivity.h"
#include "affair.h"
#include <QJsonObject>
#include "basic.h"
#include "online_data.h"
#include <QMessageBox>

addactivity::addactivity(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addactivity)
{
    ui->setupUi(this);
}

addactivity::~addactivity()
{
    delete ui;
}

void addactivity::on_add_clicked()
{
    QJsonObject a;
    open_json("place_id.json",a);
    int placeid=a[ui->place->currentText()].toInt();
    activity newa=activity(
        ui->name->text(),
        ui->tag->currentIndex()+1,
        school_online->idtopos[placeid],
        ui->time->currentIndex()+8,
        ui->time->currentIndex()+8,
        ui->day->currentIndex()+1,
        1<<ui->day->currentIndex()
        );
    if(user_online->add_activity(newa)){
        emit flash();
        this->close();
    }
    else{
        QMessageBox::critical(this, "错误","存在时间冲突，请更正！");
    }
}

