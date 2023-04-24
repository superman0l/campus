#include "setcourse.h"
#include "ui_setcourse.h"
#include "basic.h"
#include <QMessageBox>
#include "online_data.h"

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

void setcourse::receivesetData(QModelIndex indx, QString show)
{
    QStringList strlist=show.split("\n\n");
    name=strlist[0];teacher=strlist[1];day=indx.column()+1;
    ui->weekday->setCurrentIndex(indx.column());
    QString ttle="修改课程:\n"+strlist[0];
    ui->title->setText(ttle);
    int st,ed;
    qstr_to_time(strlist[3],st,ed);
    ui->startweek->setCurrentIndex(st-8);
    ui->period->setCurrentIndex(ed-st);
}

void setcourse::on_startweek_currentIndexChanged(int index)
{
    if(index>ui->endweek->currentIndex()){
        QMessageBox::critical(this, "错误","开始周数不能晚于结束周数！");
        ui->startweek->setCurrentIndex(ui->endweek->currentIndex());
    }
}


void setcourse::on_endweek_currentIndexChanged(int index)
{
    if(index<ui->startweek->currentIndex()){
        QMessageBox::critical(this, "错误","结束周数不能早于开始周数！");
        ui->endweek->setCurrentIndex(ui->startweek->currentIndex());
    }
}


void setcourse::on_set_clicked()
{
    QString classroom=ui->place->currentText()+ui->lineEdit->text();
    int start=ui->starttime->currentIndex()+8, end =ui->period->currentIndex()+start, weekday=ui->weekday->currentIndex()+1;
    int startweek=ui->startweek->currentIndex()+1, endweek=ui->endweek->currentIndex()+1;
    int placeid=qstr_to_placeid(ui->place->currentText());
    course check=course(name,school_online->idtopos[placeid],start,end,weekday,1<<(day-1),startweek,endweek,teacher,classroom);
    if(admin_online->update_course(check,admin_online->get_classid(),day)){
        QMessageBox::information(this, "提示", "更改成功");
        emit change();
        this->close();
    }
    else{
        QMessageBox::critical(this, "错误","更改的课程存在时间冲突\n请修改时间后再进行添加");
    }
}

