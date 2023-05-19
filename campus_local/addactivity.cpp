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
    int placeid;
    if(ui->place->currentText()=="非线下活动")placeid=-1;
    else placeid=a[ui->place->currentText()].toInt();
    int s=0;
    if(ui->day->currentIndex()==0){
        for(int i=0;i<7;i++)s+=1<<i;
    }
    else s=1<<(ui->day->currentIndex()-1);
    activity newa=activity(
        ui->name->text(),
        ui->tag->currentIndex()+1,
        school_online->idtopos[placeid],
        ui->time->currentIndex()+8,
        ui->time->currentIndex()+8,
        ui->day->currentIndex(),
        ui->platform->text(),
        ui->url->text(),
        s
        );
    int min;
    if(!ui->check->isChecked())min=0;
    else min=ui->min->value();
    if(user_online->add_activity(newa,min)){
        emit flash(newa.day,newa.tag,1);
        log_action(tr("添加活动%1成功").arg(newa.name));
        this->close();
    }
    else{
        QMessageBox::critical(this, "错误","存在时间冲突，请更正！");
        log_action(tr("添加活动%1失败").arg(newa.name));
    }
}


void addactivity::on_check_stateChanged(int arg1)
{
    if(ui->check->isChecked())
        ui->min->setEnabled(true);
    else
        ui->min->setEnabled(false);
}

