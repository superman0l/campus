#include "alarmwin.h"
#include "ui_alarmwin.h"
#include "timer.h"
alarmwin::alarmwin(timer*mtm,talarm mta,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::alarmwin)
{
    ui->setupUi(this);
    this->tm=mtm;
    this->ta=mta;
    this->ui->textBrowser->setText(ta.info);
}
void alarmwin::on_close_button_clicked()
{
    this->close();
}
void alarmwin::on_relay_button_clicked()
{
    if(this->ta.cnt==MAX_RELAY_CNT)
    {
        this->close();
        return;
    }
    int add=this->tm->get_ratio()/60;
    add=std::max(add,5);
    auto nta=this->ta;
    nta.cnt++;
    nta.minute+=add;
    int in=nta.minute/60;
    nta.minute%=60;
    if(in)
    {
        nta.hour+=in;
        if(nta.hour>=24)//若跨天,则放弃闹钟
        {
            this->close();
            return;
        }
    }
    this->tm->insert(nta);
    this->close();
}
alarmwin::~alarmwin()
{
    delete ui;
}
