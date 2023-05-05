#include "classwind.h"
#include "ui_classwind.h"
#include <QStandardItemModel>
#include "basic.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "online_data.h"
#include <QListWidgetItem>
#include "timer.h"

classwind::classwind(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::classwind)
{
    ui->setupUi(this);


    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

    model->setHorizontalHeaderLabels({"周一", "周二", "周三", "周四", "周五", "周六", "周日"});
    model->setVerticalHeaderLabels({"8:00-9:00", "9:00-10:00", "10:00-11:00", "11:00-12:00", "休息", "13:00-14:00", "14:00-15:00", "15:00-16:00", "16:00-17:00", "17:00-18:00", "休息", "19:00-20:00", "20:00-21:00"});
    ui->week->setCurrentIndex(tim->get_week()-1);
    load(tim->get_week());
    ui->tableView->setModel(model);
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


void classwind::load(int weeknum){
    //int weeknum=tim->get_week();

    QJsonArray courseArray=load_student_class_coursearray(QString::number(user_online->get_id()));
    QString name,teacher,al,classroom;
    int starttime,period,day;
    int stweek,edweek;
    bool alarm;
    QString platform,url;
    bool flag[91]={0};
    for(int i=0;i<courseArray.size();i++){
        QJsonObject course=courseArray.at(i).toObject();
        QString message;
        stweek=course["startweek"].toInt();
        edweek=course["endweek"].toInt();
        starttime=course["starttime"].toInt();
        day=course["weekday"].toInt();
        period=course["endtime"].toInt()-starttime+1;

        if(weeknum<stweek||weeknum>edweek){
            for(int j=0;j<period;j++){
                if(flag[(starttime-8+j)*7+day-1])break;
                model->setItem(starttime-8+j,day-1,new QStandardItem(""));
            }
        }
        else{
            name=course["name"].toString();
            teacher=course["teacher"].toString();
            if(course["destination_id"].toInt()!=-1)
                classroom=course["classroom"].toString();
            else classroom="非线下课程";
            platform=course["platform"].toString();
            url=course["url"].toString();
            alarm=course["alarm"].toObject()["enable"].toBool();
            al=alarm==true?"alarm:on":"alarm:off";
            if(url!=""&&platform!="")
                message=name+"\n\n"+teacher+"\n\n"+classroom+"\n\n"+QString::number(starttime)+":00"+"-"+QString::number(starttime+period)+":00"+"\n\n"+al+"\n\n"+platform+"\n\n"+url;
            else message=name+"\n\n"+teacher+"\n\n"+classroom+"\n\n"+QString::number(starttime)+":00"+"-"+QString::number(starttime+period)+":00"+"\n\n"+al;
            for(int j=0;j<period;j++){
                flag[(starttime-8+j)*7+day-1]=1;
                model->setItem(starttime-8+j,day-1,new QStandardItem(message));
                model->item(starttime-8+j,day-1)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
}

void classwind::on_tableView_clicked(const QModelIndex &index)
{
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    QModelIndex indx = ui->tableView->currentIndex();
    QVariant data = model->data(indx);
    QString show = data.toString();
    QStringList sl=show.split("\n\n");
    if(show.length()!=0){
        ui->checkBox->setEnabled(true);
        if(sl[2]=="非线下课程")
            ui->navigate->setEnabled(false);
        else
            ui->navigate->setEnabled(true);
        if(show.contains("on",Qt::CaseSensitive)){
            ui->checkBox->setChecked(true);
        }
        else{
            ui->checkBox->setChecked(false);
        }
    }
    else{
        ui->checkBox->setEnabled(false);
        ui->navigate->setEnabled(false);
    }
    QString underline,urls;
    for(int i=0;i<5;i++){
        underline+=sl[i]+"  ";
    }
    ui->label->setText(underline);
    if(sl.length()==7)
        urls=sl[5]+"  "+sl[6];
    ui->url->setText(urls);
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
    ui->result->clearSelection();
    ui->result->clear();
    if(ui->course_line->text()==""){
        QString mesg="无有效输入";
        QListWidgetItem * item = new QListWidgetItem;
        item->setSizeHint(QSize(ui->result->width(),20));
        item->setSizeHint(QSize(ui->result->height(),25));
        item->setText(QString(mesg));
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
        QListWidgetItem * item = new QListWidgetItem;
        item->setSizeHint(QSize(ui->result->width(),20));
        item->setSizeHint(QSize(ui->result->height(),25));
        item->setText(QString(error));
        ui->result->addItem(item);
    }
    for(int i=0;i<courses.size();i++){

        QString info=courses[i].name+"  "+num_to_qstr(courses[i].day)+"  "+QString::number(courses[i].start)+":00-"+QString::number(courses[i].end+1)+":00  "+courses[i].classroom+"  "+courses[i].teacher;
        QListWidgetItem * item = new QListWidgetItem;
        item->setSizeHint(QSize(ui->result->width(),20));
        item->setSizeHint(QSize(ui->result->height(),25));
        item->setText(QString(info));
        ui->result->addItem(item);
    }
}

void classwind::on_result_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current==NULL)return;
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView->clearSelection();
    QString data=ui->result->currentItem()->text();
    QStringList strlist=data.split("  ");
    int col=qstr_to_num(strlist[1])-1;
    int st,ed;
    qstr_to_time(strlist[2],st,ed);
    for(int i=0;i<ed-st;i++)
        ui->tableView->setCurrentIndex(ui->tableView->model()->index(st-8+i, col));
    on_tableView_clicked(ui->tableView->model()->index(st-8, col));
}


void classwind::on_navigate_clicked()
{
    int stid,edid;
    stid=user_online->get_place_id();
    edid=qstr_to_placeid(ui->label->text());
    QString text;
    if(edid!=-1)text=school_online->navigate(stid,edid);
    else text="无有效课程目的地，请重新选择";
    ui->naviresult->setText(text);
}


void classwind::on_week_currentIndexChanged(int index)
{
    load(index+1);
    ui->tableView->setModel(model);
    for(int i=0;i<13;i++){
        if(i==4 || i==10)
            ui->tableView->setRowHeight(i,20);
        else ui->tableView->setRowHeight(i,150);
    }
    for(int i=0;i<7;i++)ui->tableView->setColumnWidth(i,80);
}


void classwind::on_course_line_returnPressed()
{
    on_search_clicked();
}

