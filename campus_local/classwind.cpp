#include "classwind.h"
#include "ui_classwind.h"
#include <QStandardItemModel>
#include "basic.h"
#include <QJsonObject>
#include <QJsonArray>
#include "login.h"

classwind::classwind(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::classwind)
{
    ui->setupUi(this);
    model->setHorizontalHeaderLabels({"周一", "周二", "周三", "周四", "周五", "周六", "周日"});
    model->setVerticalHeaderLabels({"8:00-9:00", "9:00-10:00", "10:00-11:00", "11:00-12:00", "休息", "13:00-14:00", "14:00-15:00", "15:00-16:00", "16:00-17:00", "17:00-18:00", "休息", "19:00-20:00", "20:00-21:00"});
    ui->tableView->setModel(model);
    for(int i=0;i<13;i++){
        if(i==4 || i==10)
            ui->tableView->setRowHeight(i,20);
        else ui->tableView->setRowHeight(i,150);
    }
}

classwind::~classwind()
{
    delete ui;
}


void classwind::load(){
    //const User* user=per;
    QJsonObject studentObject;
    //open_json(id+".json", studentObject);
    QString classid = studentObject["class"].toString();
    QJsonObject classObject;
    open_json(classid+".json", classObject);
    QJsonArray courseArray=classObject["courses"].toArray();
    QString name,teacher;
    int starttime,period,day;
    for(int i=0;i<courseArray.size();i++){
        QJsonObject course=courseArray.at(i).toObject();
        QString
        name=course["name"].toString();
        teacher=course["course"].toString();
        starttime=course["starttime"].toInt();
        period=course["endtime"].toInt()-starttime+1;
        day=course["weekday"].toInt();
        for(int j=0;j<period;j++){
            model->setItem(starttime+j,day-1,new QStandardItem("1"));
        }
    }
}

void classwind::UserIn(const User* d)
{
    user=d;
}

