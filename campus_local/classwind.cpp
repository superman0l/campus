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
    ui->tableView->setFixedSize(671,441);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

    model->setHorizontalHeaderLabels({"周一", "周二", "周三", "周四", "周五", "周六", "周日"});
    model->setVerticalHeaderLabels({"8:00-9:00", "9:00-10:00", "10:00-11:00", "11:00-12:00", "休息", "13:00-14:00", "14:00-15:00", "15:00-16:00", "16:00-17:00", "17:00-18:00", "休息", "19:00-20:00", "20:00-21:00"});
    ui->week->setCurrentIndex(tim->get_week()-1);
    tim->load_course=false;
    load(tim->get_week());
    tim->load_course=true;
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

            //改格式傻逼问题之读不同的文件
            QJsonObject tempuser;open_json(QString::number(user_online->get_id())+".json",tempuser);bool isread=0;
            QJsonArray alarmarray=tempuser["course_alarm"].toArray();
            for(int i=0;i<alarmarray.count();i++){
                if(alarmarray[i].toObject()["name"]==name&&alarmarray[i].toObject()["alarm"].toObject()["day"].toInt()==day){
                    isread=1;
                    if(alarmarray[i].toObject()["alarm"].toObject()["enable"].toBool())alarm=true;
                    else alarm=false;
                }
            }
            if(!isread)alarm=false;

            //alarm=course["alarm"].toObject()["enable"].toBool();
            al=alarm==true?"alarm:on":"alarm:off";
            if(!(tim->load_course)&&alarm==true&&stweek<=tim->get_week()&&edweek>=tim->get_week())
            {
                QString s=QString("课程名称：%1\n教师：%2\n").arg(name).arg(teacher);
                if(course["destination_id"].toInt()!=-1)
                {
                    s=s+QString("上课地点：%1\n导航路线：%2").arg(classroom).arg(school_online->navigate(user_online->get_place_id(),course["destination_id"].toInt()));
                }else
                {
                    s=s+QString("上课平台：%1\n上课链接：%2").arg(platform).arg(url);
                }
                tim->insert(talarm(day,starttime-1,30,(1<<(day-1)),s));//插入闹钟
            }else if(!(tim->load_course))
            {
                QString s=QString("课程名称：%1\n教师：%2\n").arg(name).arg(teacher);
                if(course["destination_id"].toInt()!=-1)
                {
                    s=s+QString("上课地点：%1\n导航路线：%2").arg(classroom).arg(school_online->navigate(user_online->get_place_id(),course["destination_id"].toInt()));
                }else
                {
                    s=s+QString("上课平台：%1\n上课链接：%2").arg(platform).arg(url);
                }
                tim->erase(talarm(day,starttime-1,30,(1<<(day-1)),s));//插入闹钟
            }
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
        QString underline,urls;
        for(int i=0;i<5;i++){
            underline+=sl[i]+"  ";
        }
        ui->label->setText(underline);
        if(sl.length()==7)
            urls=sl[5]+"  "+sl[6];
        ui->url->setText(urls);
    }
    else{
        ui->checkBox->setEnabled(false);
        ui->navigate->setEnabled(false);
    }

}


void classwind::on_checkBox_stateChanged(int arg1)
{
    QString coursename;
    QString data= model->data(ui->tableView->currentIndex()).toString();
    QStringList datalist=data.split("\n\n");
    coursename=datalist[0];
    QJsonObject course1=load_course_json(QString::number(user_online->get_id()),coursename,ui->tableView->currentIndex().column()+1);
    course course2= jsontocourse(course1);
    QModelIndex indx=ui->tableView->currentIndex();
    if(ui->checkBox->isChecked()){
        user_online->set_clock_course(course2,true);
        log_action(tr("设置%1的闹钟").arg(course2.name));
        //load(tim->get_week());
        /*
        QString newdata=datalist[0]+"\n\n"+datalist[1]+"\n\n"+datalist[2]+"\n\n"+datalist[3]+"\n\n"+"alarm:on";
        for(int j=0;j<course2.end-course2.start+1;j++){
            model->setItem(course2.start-8+j,course2.day-1,new QStandardItem(newdata));
            model->item(course2.start-8+j,course2.day-1)->setTextAlignment(Qt::AlignCenter);
        }
        ui->tableView->setModel(model);
        QString newunderdata=datalist[0]+"  "+datalist[1]+"  "+datalist[2]+"  "+datalist[3]+"  "+"alarm:on";
        ui->label->setText(newunderdata);*/
    }
    else{
        user_online->set_clock_course(course2,false);

        log_action(tr("取消%1的闹钟").arg(course2.name));
        /*
        QString newdata=datalist[0]+"\n\n"+datalist[1]+"\n\n"+datalist[2]+"\n\n"+datalist[3]+"\n\n"+"alarm:off";
        for(int j=0;j<course2.end-course2.start+1;j++){
            model->setItem(course2.start-8+j,course2.day-1,new QStandardItem(newdata));
            model->item(course2.start-8+j,course2.day-1)->setTextAlignment(Qt::AlignCenter);
        }
        ui->tableView->setModel(model);
        QString newunderdata=datalist[0]+"  "+datalist[1]+"  "+datalist[2]+"  "+datalist[3]+"  "+"alarm:off";
        ui->label->setText(newunderdata);*/
    }
    ui->week->setCurrentIndex(tim->get_week()-1);
    on_week_currentIndexChanged(tim->get_week()-1);
    on_tableView_clicked(indx);
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
        ui->result->setEnabled(false);
        return;
    }
    else{
        ui->result->setEnabled(true);
    }
    int tag;
    if(ui->comboBox->currentText()=="搜索课程名称")
        tag=0;
    else if(ui->comboBox->currentText()=="搜索教师名称")
        tag=1;
    std::vector<course> courses=user_online->query(ui->course_line->text(),school_online,tag);
    log_action(tr("搜索%1：%2").arg(tag?"课程名称":"教师名称").arg(ui->course_line->text()));
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

    log_action(tr("查询从%1到%2的最佳路线").arg(school_online->idtopos[stid].name).arg(school_online->idtopos[edid].name));
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

