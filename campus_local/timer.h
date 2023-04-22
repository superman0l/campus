#ifndef TIMER_H
#define TIMER_H
#include<QDateTime>
#include<ctime>
///
/// @brief The timer class 继承QDateTime
/// @brief QDateTime相关: https://doc.qt.io/qt-6/qdatetime.html
class timer:public QDateTime
{
private:
        QDateTime term_begin;//记录学期开始时间点,默认为2023年9月5日
        /// @brief last记录时间点
        clock_t last;
        /// @brief ratio记录时间比率
        int ratio;
        /// @brief is_paused 表示时间是否暂停
        bool is_paused;
public:
    timer():QDateTime(QDateTime::currentDateTime()),last(clock()),ratio(360),is_paused(false),term_begin(QDateTime::fromString(QString("20230905000000"),QString("yyyyMMddHHmmss"))){}
    timer(QDateTime qt,clock_t tnow,int tratio,bool this_paused):QDateTime(qt),last(tnow),ratio(tratio),is_paused(this_paused),term_begin(QDateTime::fromString(QString("20230905000000"),QString("yyyyMMddHHmmss"))){}

    /// @brief puase 设置暂停
    void puase(){ is_paused=true;}

    /// @brief begin 设置启动
    void begin(){is_paused=false;}

    /// @brief set_ratio 设置时间倍率
    /// @param nratio 新倍率
    /// @return 设置成功返回true,设置失败返回false(新倍率小于等于0)
    bool set_ratio(double nratio);

    double get_ratio(){return ratio;}

    /// @brief update 根据clock()和is_paused更新时间
    void update();

    /// @brief output 输出当前时间(Debug中使用)
    void output();

    /// @brief set_term_begin 设置学期开始时间点
    /// @param qs 以字符串形式传入时间点,形式为"yyyyMMddHHmmss",如"20220905000000"
    void set_term_begin(QString qs);

    /// @brief get_week 获取当前时间点对应周数(从1开始),如第1周,第2周等
    /// @return 周数
    int get_week();
};
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
#endif // TIMER_H
