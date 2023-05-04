#include "addexam.h"
#include "ui_addexam.h"
#include <map>
#include <QJsonObject>
#include "online_data.h"
#include "basic.h"
#include <QMessageBox>
addexam::addexam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addexam)
{
    ui->setupUi(this);
    std::map<QString, bool> check;
    QJsonObject clas;
    open_json(QString::number(admin_online->get_classid())+"_course.json",clas);
    QJsonArray courses=clas["courses"].toArray();
    for(int i=0;i<courses.size();i++){
        QString name=courses.at(i).toObject()["name"].toString();
        if(check[name]==0){
            check[name]=1;
            int k=ui->name->count();
            ui->name->insertItem(k,name);
        }
        else
            continue;
    }
}

addexam::~addexam()
{
    delete ui;
}

void addexam::on_set_clicked()
{
    QString name=ui->name->currentText()+"-考试",classroom=ui->place->currentText()+ui->classroom->text(),teacher="";
    int start;
    if(ui->start->currentIndex()>8)
        start=ui->start->currentIndex()+10;
    else if(ui->start->currentIndex()>3)
        start=ui->start->currentIndex()+9;
    else
        start=ui->start->currentIndex()+8;
    int end =ui->period->currentIndex()+start, day=ui->weekday->currentIndex()+1;
    int startweek=ui->week->currentIndex()+1, endweek=startweek;
    int placeid=qstr_to_placeid(ui->place->currentText());
    course check=course(
        name,
        school_online->idtopos[placeid],
        start,
        end,
        day,
        1<<(day-1),
        startweek,
        endweek,
        teacher,
        classroom,
        "",
        "");
    QString classid =QString::number(admin_online->get_classid());
    QJsonObject classObject;open_json(classid+"_course.json",classObject);
    QJsonArray coursearray=classObject["courses"].toArray();
    if(admin_online->add_course(check,admin_online->get_classid())){
        QMessageBox::information(this, "提示", "发布考试成功");
        emit change();
        this->close();
    }
    else
        QMessageBox::critical(this, "错误","发布的考试存在时间冲突\n请修改时间后再进行添加");
}

