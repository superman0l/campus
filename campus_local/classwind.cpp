#include "classwind.h"
#include "ui_classwind.h"
#include <QStandardItemModel>
#include "basic.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "online_data.h"
#include <QListWidgetItem>

classwind::classwind(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::classwind)
{
    ui->setupUi(this);
    model->setHorizontalHeaderLabels({"周一", "周二", "周三", "周四", "周五", "周六", "周日"});
    model->setVerticalHeaderLabels({"8:00-9:00", "9:00-10:00", "10:00-11:00", "11:00-12:00", "休息", "13:00-14:00", "14:00-15:00", "15:00-16:00", "16:00-17:00", "17:00-18:00", "休息", "19:00-20:00", "20:00-21:00"});
    load();
    ui->tableView->setModel(model);
    qDebug()<<"111";
    for(int i=0;i<13;i++){
        if(i==4 || i==10)
            ui->tableView->setRowHeight(i,20);
        else ui->tableView->setRowHeight(i,150);
    }
    for(int i=0;i<7;i++)ui->tableView->setColumnWidth(i,80);

    //ui->tableView->installEventFilter(this);
}

classwind::~classwind()
{
    delete ui;
}


void classwind::load(){
    QJsonArray courseArray=load_student_class_coursearray(QString::number(user_online->get_id()));
    QString name,teacher,al;
    int starttime,period,day;
    bool alarm;
    for(int i=0;i<courseArray.size();i++){
        QJsonObject course=courseArray.at(i).toObject();
        QString message;
        name=course["name"].toString();
        teacher=course["teacher"].toString();
        starttime=course["starttime"].toInt();
        period=course["endtime"].toInt()-starttime+1;
        day=course["weekday"].toInt();
        alarm=course["alarm"].toObject()["enable"].toBool();
        al=alarm==true?"alarm:on":"alarm:off";
        message=name+"\n\n"+teacher+"\n\n"+QString::number(starttime)+":00"+"-"+QString::number(starttime+period)+":00"+"\n\n"+al;
        for(int j=0;j<period;j++){
            model->setItem(starttime-8+j,day-1,new QStandardItem(message));
            model->item(starttime-8+j,day-1)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void classwind::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indx = ui->tableView->currentIndex();
    QVariant data = model->data(indx);
    QString show = data.toString();
    if(show.length()!=0){
        ui->checkBox->setEnabled(true);
        if(show.contains("on",Qt::CaseSensitive)){
            ui->checkBox->setChecked(true);
        }
        else{
            ui->checkBox->setChecked(false);
        }
    }
    else{
        ui->checkBox->setEnabled(false);
    }

    for(int i=0;i<show.length();i++){
        if(show[i]=='\n')show[i]=' ';
    }
    ui->label->setText(show);
}


void classwind::on_checkBox_stateChanged(int arg1)
{
    QString coursename;
    QString data= model->data(ui->tableView->currentIndex()).toString();
    QStringList datalist=data.split("\n\n");
    coursename=datalist[0];
    QJsonObject course1=load_course_json(QString::number(user_online->get_id()),coursename);
    course course2= jsontocourse(course1,school_online);
    if(ui->checkBox->isChecked()){
        user_online->set_clock_course(course2,true);
        QString newdata=datalist[0]+"\n\n"+datalist[1]+"\n\n"+datalist[2]+"\n\n"+"alarm:on";
        for(int j=0;j<course2.end-course2.start+1;j++){
            model->setItem(course2.start-8+j,course2.day-1,new QStandardItem(newdata));
            model->item(course2.start-8+j,course2.day-1)->setTextAlignment(Qt::AlignCenter);
        }
        ui->tableView->setModel(model);
        QString newunderdata=datalist[0]+"  "+datalist[1]+"  "+datalist[2]+"  "+"alarm:on";
        ui->label->setText(newunderdata);
    }
    else{
        user_online->set_clock_course(course2,false);
        QString newdata=datalist[0]+"\n\n"+datalist[1]+"\n\n"+datalist[2]+"\n\n"+"alarm:off";
        for(int j=0;j<course2.end-course2.start+1;j++){
            model->setItem(course2.start-8+j,course2.day-1,new QStandardItem(newdata));
            model->item(course2.start-8+j,course2.day-1)->setTextAlignment(Qt::AlignCenter);
        }
        ui->tableView->setModel(model);
        QString newunderdata=datalist[0]+"  "+datalist[1]+"  "+datalist[2]+"  "+"alarm:off";
        ui->label->setText(newunderdata);
    }
}


void classwind::on_search_clicked()
{
    ui->result->clear();
    if(ui->course_line->text()==""){
        int itemCount = ui->result->count();
        QString mesg="无有效输入";
        QListWidgetItem * item = new QListWidgetItem;
        item->setSizeHint(QSize(ui->result->width(),20));
        item->setSizeHint(QSize(ui->result->height(),25));
        item->setText(QString(mesg).arg(itemCount));
        ui->result->addItem(item);
        return;
    }
    int tag;
    if(ui->comboBox->currentText()=="搜索课程名称")
        tag=0;
    else if(ui->comboBox->currentText()=="搜索教师名称")
        tag=1;
    std::vector<course> courses=user_online->query(ui->course_line->text(),school_online,tag);
    if(courses.empty()){
        QString error="无结果。请检查搜索内容";
        int itemCount = ui->result->count();
        QListWidgetItem * item = new QListWidgetItem;
        item->setSizeHint(QSize(ui->result->width(),20));
        item->setSizeHint(QSize(ui->result->height(),25));
        item->setText(QString(error).arg(itemCount));
        ui->result->addItem(item);
    }
    for(int i=0;i<courses.size();i++){

        QString info=courses[i].name+"  "+num_to_qstr(courses[i].day)+"  "+QString::number(courses[i].start)+":00-"+QString::number(courses[i].end)+":00  "+courses[i].teacher;

        int itemCount = ui->result->count();
        QListWidgetItem * item = new QListWidgetItem;
        item->setSizeHint(QSize(ui->result->width(),20));
        item->setSizeHint(QSize(ui->result->height(),25));
        item->setText(QString(info).arg(itemCount));
        ui->result->addItem(item);
    }
}

