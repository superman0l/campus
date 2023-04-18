#ifndef AFFAIR_H
#define AFFAIR_H

#include<bits/stdc++.h>
#include "map.h"

constexpr int Monday = (1);
constexpr int Tuesday = (1 << 1);
constexpr int Wednesday = (1 << 2);
constexpr int Thursday = (1 << 3);
constexpr int Friday = (1 << 4);
constexpr int Saturday = (1 << 5);
constexpr int Sunday = (1 << 6);

class affair
{

public:
    int start, end, day;//开始、结束时间（小时），周几
    int period;//是否重复
    position place;
    QString name;
    affair(QString name, position place, int start_time, int end_time, int day, int periodicity = 0)
        : name(name), place(place), start(start_time), end(end_time), day(day), period(periodicity){};
    virtual ~affair(){};

    /// @brief 将活动信息写入文件
    /// @param fname 文件名
    /// @return 返回是否成功写入
    //virtual bool write_in(const std::string fname) = 0;
};
class course : public affair
{
public:
    int start_week;//第几周开始
    int end_week;
    QString teacher;

    course(QString name, position place, int start_time, int end_time, int day, int periodicity, int startweek, int endweek, QString teacher)
        : affair(name, place, start_time, end_time, day, periodicity), start_week(startweek), end_week(endweek), teacher(teacher){};
    virtual ~course(){};
    //virtual bool write_in(const std::string fname);
};
class activity : public affair
{

public:
    int tag;//activity 1-7 自习 锻炼 外出 班会 小组作业 双创 聚餐

    activity(QString name, int tag, position place, int start_time, int end_time, int day, int periodicity = 0)
        : affair(name, place, start_time, end_time, day, periodicity), tag(tag){};
    virtual ~activity(){};

    /// @brief 设置tag属性
    /// @param st 要添加的tag
    void set_tag(int st)
    {
        tag|=st;
    }

    /// @brief 消除tag属性
    /// @param et 要消除的tag
    void erase_tag(int et)
    {
        tag&=(~et);
    }
    //virtual bool write_in(const std::string fname);
};
class tmpaffair : public activity
{
public:
    int tag;//tmpaffair 1-4 购物 洗澡 取物 吃饭
    tmpaffair(QString name, int tag, position place, int start_time, int end_time, int day, int periodicity = 0)
        : activity(name, tag, place, start_time, end_time, day, periodicity){};
    virtual ~tmpaffair(){};

};


#endif // AFFAIR_H
