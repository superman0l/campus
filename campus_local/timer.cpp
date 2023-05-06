#include "timer.h"
#include"alarmwin.h"
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
        this->erase(e);
        auto it=new alarmwin(this,e,this->parent);
        it->setAttribute(Qt::WA_DeleteOnClose);
        it->show();
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
