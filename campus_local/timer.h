#ifndef TIMER_H
#define TIMER_H
#include<QDateTime>
#include<ctime>
#include <queue>
#include<set>
static const QString st_time="20230220000000";
constexpr int MAX_RELAY_CNT=5;
class talarm
{
public:
    int day;//闹钟时间
    int hour;
    int minute;
    int periodicity;//闹钟重复周期
    int cnt;//表示闹钟已经被推迟几次,达到上限后不再推迟
    QString info;//闹钟相关信息,如导航路线,上课链接等
    talarm(){}
    talarm(int day,int hour,int minute,int periodicity,QString info):day(day),hour(hour),minute(minute),periodicity(periodicity),info(info),cnt(0){}
    bool operator < (const talarm& rhs)const
    {
        if(day!=rhs.day)return day<rhs.day;
        if(hour!=rhs.hour) return hour<rhs.hour;
        if(minute!=rhs.minute) return minute<rhs.minute;
        if(periodicity!=rhs.periodicity) return periodicity<rhs.periodicity;
        return info<rhs.info;
    }
};

///
/// @brief The timer class
class timer
{
private:
        QWidget*parent;
        QDateTime now;//记录当前时间
        QDateTime term_begin;//记录学期开始时间点,默认为2023年2月20日
        /// @brief last记录时间点
        clock_t last;
        /// @brief ratio记录时间比率
        int ratio;
        /// @brief is_paused 表示时间是否暂停
        bool is_paused;
        std::vector<std::set<talarm>>alarm_st;
public:
    timer(QWidget*parent=nullptr):now(QDateTime::currentDateTime()),last(clock()),ratio(360),is_paused(false),term_begin(QDateTime::fromString(st_time,QString("yyyyMMddHHmmss"))),alarm_st(std::vector<std::set<talarm>>(8)),parent(parent){}
    timer(QDateTime qt,clock_t tnow,int tratio,bool this_paused,QWidget*parent=nullptr):now(qt),last(tnow),ratio(tratio),is_paused(this_paused),term_begin(QDateTime::fromString(st_time,QString("yyyyMMddHHmmss"))),parent(parent){}

    /// @brief puase 设置暂停
    void puase(){ is_paused=true;}

    /// @brief begin 设置启动
    void begin(){is_paused=false;}

    /// @brief set_ratio 设置时间倍率
    /// @param nratio 新倍率
    /// @return 设置成功返回true,设置失败返回false(新倍率小于等于0)
    bool set_ratio(double nratio);

    ///@brief set_Date 更新日期
    /// @param date 一个QDate类型的引用，用以更新日期
    void set_Date(QDate& date);

    ///@brief set_Time 更新时间
    /// @param date 一个QTime类型的引用，用以更新时间
    void set_Time(QTime& time);

    double get_ratio(){return ratio;}

    bool get_paused(){return is_paused;}

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

    /// @brief get_days 获取当前时间点对应星期几,如1表示星期1,7表示星期天
    /// @return 星期几
    int get_days();

    /// @brief insert 插入闹钟
    /// @param ta 闹钟
    /// @return 是否插入成功
    bool insert(talarm ta);

    bool erase(talarm ta);
    QString toString(const QString&);
    const QDateTime& get_DateTime(){return now;}
};

#endif // TIMER_H
