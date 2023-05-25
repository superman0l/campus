#include "addtmpaffair.h"
#include "ui_addtmpaffair.h"
#include "affair.h"
#include <QJsonObject>
#include "basic.h"
#include "online_data.h"
#include <QMessageBox>

addtmpaffair::addtmpaffair(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addtmpaffair)
{
    ui->setupUi(this);
}

addtmpaffair::~addtmpaffair()
{
    delete ui;
}

void addtmpaffair::on_add_clicked()
{
    QJsonObject m;
    open_json("place_id.json",m);
    int placeid=m[ui->place->currentText()].toInt();
    tmpaffair t=tmpaffair(
        ui->name->text(),
        ui->tag->currentIndex()+1,
        school_online->idtopos[placeid],
        ui->time->currentIndex()+6,
        ui->time->currentIndex()+6,
        tim->get_days(),
        1<<(tim->get_days()-1)
        );
    if(user_online->add_tmpaffair(t)){
        emit flash(ui->tag->currentIndex()+1,1);
        log_action(tr("添加临时事务%1成功").arg(t.name));
        this->close();
    }
    else{
        log_action(tr("添加临时事务%1失败").arg(t.name));
        QMessageBox::critical(this, "错误","存在时间冲突，请更正！");
    }
}

