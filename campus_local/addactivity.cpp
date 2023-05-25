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
    int placeid;
    if(ui->place->currentText()=="非线下活动")placeid=-1;
    else placeid=school_online->get_id(ui->place->currentText());;
    int s=0;
    if(ui->day->currentIndex()==0){
        for(int i=0;i<7;i++)s+=1<<i;
    }
    else s=1<<(ui->day->currentIndex()-1);
    activity newa=activity(
        ui->name->text(),
        ui->tag->currentIndex()+1,
        placeid==-1?position(-1,""):school_online->idtopos[placeid],
        ui->time->currentIndex()+6,
        ui->time->currentIndex()+6,
        ui->day->currentIndex(),
        ui->platform->text(),
        ui->url->text(),
        s
        );
    int min;
    if(!ui->check->isChecked())min=0;
    else min=ui->min->value();

    bool isavailable=1;
    if(ui->day->currentIndex()==0){
        for(int i=0;i<7;i++){
            if(timetable_online[ui->day->currentIndex()-1][ui->time->currentIndex()]>1){
                isavailable=0;
                break;
            }
        }
    }
    else if(timetable_online[ui->day->currentIndex()-1][ui->time->currentIndex()]>1){
        isavailable=0;
    }
    if(isavailable){
        //集体活动检测
        for(int i=0;i<checktables.size();i++){
            if(checktables[i][ui->day->currentIndex()-1][ui->time->currentIndex()].toInt()>1){
                std::vector<int> enabletime;
                for(int j=0;j<16;j++){
                    int x=timetable_online[ui->day->currentIndex()-1][j];
                    for(int k=0;k<checktables.size();k++)
                        x|=checktables[k][ui->day->currentIndex()-1][j].toInt();
                    if(x<=1)enabletime.push_back(j+6);
                }
                if(enabletime.size()==0){
                    QMessageBox::critical(this, "错误","集体时间冲突并且当天内无可用时间，请择日开展");
                }
                else{
                    QString msg="集体时间冲突,当天可用时间为：\n";
                    for(int i=0;i<enabletime.size();i++){
                        msg+=QString::number(enabletime[i])+":00-"+QString::number(enabletime[i]+1)+":00\n";
                    }
                    QMessageBox::critical(this, "错误",msg);
                }
                return;
            }
        }

        user_online->add_activity(newa,min);

        if(checktables.size()>0){//集体活动集体添加
            for(int i=0;i<checktables.size();i++){
                QString otherid=ui->idlist->itemText(i);
                QJsonObject other;open_json(otherid+".json",other);
                QJsonArray otherarray=other["activities"].toArray();
                QJsonArray tablearray=other["timetable"].toArray();
                int a=tablearray[ui->day->currentIndex()-1][ui->time->currentIndex()].toInt();
                tablearray[ui->day->currentIndex()-1].toArray().replace(ui->time->currentIndex(),a|2);
                otherarray.append(activitytojson(newa));
                other["activities"]=otherarray;
                other["timetable"]=tablearray;
                write_json(otherid+".json",other);
            }
        }

        emit flash(newa.day,newa.tag,1);
        emit flash_2(0, 1);
        log_action(tr("添加活动%1成功").arg(newa.name));
        this->close();
        if(min)
        {
            log_action(tr("为活动%1设置闹钟").arg(newa.name));
        }
    }
    else{
        QMessageBox::critical(this, "错误","存在个人时间冲突，请更正！");
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


void addactivity::on_tag_currentIndexChanged(int index)
{
    if(index>2){
        if(ui->day->currentIndex()==0){
            ui->tag->setCurrentIndex(0);
            QMessageBox::critical(this, "错误","集体活动不能设置为每天，请重新设置");
        }
        ui->idlist->setEnabled(true);
        ui->student_id->setEnabled(true);
        ui->add_2->setEnabled(true);
    }
    else{
        ui->idlist->setEnabled(false);
        ui->student_id->setEnabled(false);
        ui->add_2->setEnabled(false);
    }
}


void addactivity::on_add_2_clicked()
{
    QString invite_id=ui->student_id->text();
    QJsonObject other;
    if(!open_json(invite_id+".json",other)){
        QMessageBox::critical(this, "错误","系统中不存在该用户，请重新确认");
        return;
    }
    else{
        ui->idlist->addItem(invite_id);
        checktables.push_back(other["timetable"].toArray());
    }
}


void addactivity::on_day_currentIndexChanged(int index)
{
    if(index==0&&ui->tag->currentIndex()>2){
        ui->day->setCurrentIndex(1);
        QMessageBox::critical(this, "错误","集体活动不能设置为每天，请重新设置");
    }
}

