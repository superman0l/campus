#include "timer.h"
bool timer::set_ratio(double nratio){
    if(nratio>0)
    {
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
    clock_t now=clock();
    *this=timer(addSecs(1L*ratio*(now-last)/CLOCKS_PER_SEC),now,ratio,is_paused);
}
void timer::output()
{
    qDebug() << this->toString(" yyyy-MM-dd hh:mm:ss");
}
void timer::set_term_begin(QString qs)
{
    this->term_begin=QDateTime::fromString(qs,"yyyyMMddhhmmss");
}
int timer::get_week()
{
    int dif=this->term_begin.daysTo(*this);
    return 1+dif/7;
}
