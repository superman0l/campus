#include "timer.h"
#include <QDate>
#include<QMessageBox>
#include<QString>
#include<QDialog>
#include<QSoundEffect>
#include"online_data.h"
#include"basic.h"
bool timer::set_ratio(double nratio){
    if(nratio>0)
    {
        update();
        ratio=nratio;
        return true;
    }else
    {
        return false;
    }
}
void timer::set_Date(QDate &date)
{
    now.setDate(date);
}

void timer::set_Time(QTime &time)
{
    now.setTime(time);
}
void timer::update()
{
    if(is_paused){
        last=clock();
        return;
    }
    clock_t tmp=clock();
    auto [d1,h1,m1]=std::make_tuple(this->get_days(),this->now.time().hour(),this->now.time().minute());
    this->now=this->now.addSecs(1L*ratio*(tmp-this->last)/CLOCKS_PER_SEC);
    auto [d2,h2,m2]=std::make_tuple(this->get_days(),this->now.time().hour(),this->now.time().minute());
    std::vector<talarm>toshow;
    bool flg=false;
    if(d1==d2)
    {
        if((h1<22||(h1==22&&m1<=30))&&(22<h2||(22==h2&&30<m2)))
        {
            flg=true;
        }
    }else
    {
        if(h1<22||(h1==22&&m1<=30))
        {
            flg=true;
        }
        if(22<h2||(22==h2&&30<m2))
        {
            flg=true;
        }
    }
    //22:30进行询问
    std::vector<course>next_day;
    if(d1!=d2)
    {
        if(tim->affwin)
        {
            tim->load_affair=false;
            tim->affwin->load_affair(0,0);
            tim->load_affair=true;
        }
    }
    if(flg)
    {
        if(tim->has_affair)
        {
            tim->del_affair=true;
            tim->load_affair=true;
            tim->affwin->load_affair(0,0);
            tim->del_affair=false;
            if(QMessageBox::question(NULL,QString("今天即将结束"),QString("是否清空本日临时事务?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
            {
                QJsonObject stuobject,newobject;
                open_json(QString::number(user_online->get_id())+".json",stuobject);
                newobject.insert("activities",stuobject["activities"].toArray());
                newobject.insert("affairs",QJsonArray());
                newobject.insert("class",stuobject["class"].toString());
                newobject.insert("course_alarm",stuobject["course_alarm"]);
                write_json(QString::number(user_online->get_id())+".json",newobject);
                if(tim->affwin)
                {
                    tim->affwin->load_affair(0,0);
                }
            }
        }
        QJsonArray courseArray=load_student_class_coursearray(QString::number(user_online->get_id()));
        for(int i=0;i<courseArray.size();i++)
        {
            course tmp= jsontocourse(courseArray.at(i).toObject(),school_online);
            if(tmp.end_week>=tim->get_week()&&tmp.start_week<=tim->get_week()&&tmp.day==(d1%7+1))
            {
                next_day.push_back(tmp);
            }
        }
        QJsonObject stuobject;
        open_json(QString::number(user_online->get_id())+".json",stuobject);
        QJsonArray actvtarray=stuobject["activities"].toArray();
        std::vector<activity>next_act;
        for(int i=0;i<actvtarray.size();i++)
        {
            activity act=jsontoactivity(actvtarray.at(i).toObject(),school_online);
            if(act.day==0||act.day==(d1%7+1))
            {
                next_act.push_back(act);
            }
        }
        QString s;
        if(next_day.size())
        {
            s=s+QString("明日课程如下：");
            for(auto&e:next_day)
            {
                s=s+QString("\n");
                if(e.start<10)
                {
                    s=s+("0");
                }
                s=s+QString("%1:00-").arg(e.start);
                if(e.end<10)
                {
                    s=s+("0");
                }
                s=s+QString("%1:00 %2").arg(e.end).arg(e.name);
            }
        }
        if(next_act.size())
        {
            if(next_day.size())
            {
                s=s+QString("\n");
            }
            s=s+QString("明日活动如下：");
            for(auto&e:next_act)
            {

                s=s+QString("\n");
                if(e.start<10)
                {
                    s=s+("0");
                }
                s=s+QString("%1:00-").arg(e.start);
                if(e.end<10)
                {
                    s=s+("0");
                }
                s=s+QString("%1:00 %2").arg(e.end).arg(e.name);
            }
        }
        if(next_day.size()||next_act.size())
        {
            QMessageBox::information(NULL, QString("明日安排"), s, QMessageBox::Yes);
            log_event(QString("提醒第二天课程及活动%1").arg(s));
        }
    }
    if(d1==d2)
    {
        for(auto&e:this->alarm_st[d1])
        {

            if((h1<e.hour||(h1==e.hour&&m1<=e.minute))&&(e.hour<h2||(e.hour==h2&&e.minute<m2)))
            {
                toshow.push_back(e);
            }
        }
    }else
    {
        for(auto&e:this->alarm_st[d1])
        {
            if(h1<e.hour||(h1==e.hour&&m1<=e.minute))
            {
                toshow.push_back(e);
            }

        }
        for(auto&e:this->alarm_st[d2])
        {
            if(e.hour<h2||(e.hour==h2&&e.minute<m2))
            {
                toshow.push_back(e);
            }
        }
    }
    for(auto&e:toshow)
    {
        if(!e.periodicity)
        {
            this->erase(e);
        }
        bell.play();
        QMessageBox::information(NULL, QString("闹钟"), e.info, QMessageBox::Yes);
        log_event(QString("闹钟%1").arg(e.info));
    }
    last=tmp;
}
void timer::output()
{
    qDebug() << this->now.toString(" yyyy-MM-dd hh:mm:ss");
}
void timer::set_term_begin(QString qs)
{
    this->term_begin=QDateTime::fromString(qs,"yyyyMMddhhmmss");
}
int timer::get_week()
{
    int dif=this->term_begin.daysTo(this->now);
    return 1+dif/7;
}
int timer::get_days()
{
    return this->now.date().dayOfWeek();
}
QString timer::toString(const QString&format)
{
    return this->now.toString(format);
}

bool timer::insert(talarm ta)
{
    if(this->alarm_st[ta.day].count(ta))return false;
    this->alarm_st[ta.day].insert(ta);
    return true;
}
bool timer::erase(talarm ta)
{
    if(!this->alarm_st[ta.day].count(ta))return false;
    this->alarm_st[ta.day].erase(ta);
    return true;
}
