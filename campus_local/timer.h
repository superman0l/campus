#ifndef TIMER_H
#define TIMER_H
#include<QDatetime>
#include<ctime>
///
/// @brief The timer class 继承QDatetime
/// @brief QDatetime相关: https://doc.qt.io/qt-6/qdatetime.html
class timer:public QDateTime
{
private:
        /// @brief last记录时间点
        clock_t last;
        /// @brief ratio记录时间比率
        int ratio;
        /// @brief is_paused 表示时间是否暂停
        bool is_paused;
public:
    timer():QDateTime(QDateTime::currentDateTime()),last(clock()),ratio(360),is_paused(false){}
    timer(QDateTime qt,clock_t tnow,int tratio,bool tis_paused):QDateTime(qt),last(tnow),ratio(tratio),is_paused(tis_paused){}

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
#endif // TIMER_H
