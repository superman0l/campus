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
        if((h1<23||(h1==23&&m1<=0))&&(23<h2||(23==h2&&0<m2)))
        {
            flg=true;
        }
        for(auto&e:this->alarm_st[d1])
        {

            if((h1<e.hour||(h1==e.hour&&m1<=e.minute))&&(e.hour<h2||(e.hour==h2&&e.minute<m2)))
            {
                toshow.push_back(e);
            }
        }
    }else
    {
        if(h1<23||(h1==23&&m1<=0))
        {
            flg=true;
        }
        if(23<h2||(23==h2&&0<m2))
        {
            flg=true;
        }
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
    //统一晚上23:00进行提醒
    std::vector<course>next_day;
    if(flg)
    {
        QJsonArray courseArray=load_student_class_coursearray(QString::number(user_online->get_id()));
        for(int i=0;i<courseArray.size();i++)
        {
            course tmp= jsontocourse(courseArray.at(i).toObject(),school_online);
            if(tmp.end_week>=tim->get_week()&&tmp.start_week<=tim->get_week()&&tmp.day==(d1%7+1))
            {
                next_day.push_back(tmp);
            }
        }
        if(next_day.size())
        {
            QString s;
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
            QMessageBox::information(NULL, QString("明日课程"), s, QMessageBox::Yes);
            log_event(QString("提醒第二天课程%1").arg(s));
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
