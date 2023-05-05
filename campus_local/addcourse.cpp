#include "addcourse.h"
#include "campus_local/affair.h"
#include "ui_addcourse.h"
#include "online_data.h"
#include <QMessageBox>

addcourse::addcourse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addcourse)
{
    ui->setupUi(this);
}

addcourse::~addcourse()
{
    delete ui;
}

void addcourse::on_check_clicked()
{
    QString name=ui->coursename->text(),classroom=ui->classroom->text(),teacher=ui->teacher->text();
    int start;
    if(ui->start->currentIndex()>8)
        start=ui->start->currentIndex()+10;
    else if(ui->start->currentIndex()>3)
        start=ui->start->currentIndex()+9;
    else
        start=ui->start->currentIndex()+8;
    int end =ui->period->currentIndex()+start, day=ui->day->currentIndex()+1;
    int startweek=ui->startweek->currentIndex()+1, endweek=ui->endweek->currentIndex()+1;
    int placeid=qstr_to_placeid(ui->placeid->currentText());
    classroom=ui->placeid->currentText()+classroom;
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
        ui->platform->text(),
        ui->url->text()
        );
    QString classid =QString::number(admin_online->get_classid());
    QJsonObject classObject;open_json(classid+"_course.json",classObject);
    QJsonArray coursearray=classObject["courses"].toArray();
    if(admin_online->add_course(check,admin_online->get_classid())){
        QMessageBox::information(this, "提示", "添加成功");
        emit change();
        this->close();
    }
    else
        QMessageBox::critical(this, "错误","添加的课程存在时间冲突\n请修改时间后再进行添加");
}


void addcourse::on_endweek_currentIndexChanged(int index)
{
    if(index<ui->startweek->currentIndex()){
        QMessageBox::critical(this, "错误","结束周数不能早于开始周数！");
        ui->endweek->setCurrentIndex(ui->startweek->currentIndex());
    }
}


void addcourse::on_startweek_currentIndexChanged(int index)
{
    if(index>ui->endweek->currentIndex()){
        QMessageBox::critical(this, "错误","开始周数不能晚于结束周数！");
        ui->startweek->setCurrentIndex(ui->endweek->currentIndex());
    }
}

