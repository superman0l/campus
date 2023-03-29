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
private:
    int start, end, day;
    int period;
    position place;

public:
    affair(position place, int start_time, int end_time, int day, int periodicity = 0)
        : place(place), start(start_time), end(end_time), day(day), period(periodicity){};
    virtual ~affair(){};

    /// @brief 将活动信息写入文件
    /// @param fname 文件名
    /// @return 返回是否成功写入
    virtual bool write_in(const std::string fname) = 0;
};
class course : public affair
{
public:
    course(position place, int start_time, int end_time, int day, int periodicity = 0)
        : affair(place, start_time, end_time, day, periodicity){};
    virtual ~course(){};
    virtual bool write_in(const std::string fname);
};
class activity : public affair
{
private:
    int tag;

public:
    activity(int tag, position place, int start_time, int end_time, int day, int periodicity = 0)
        : affair(place, start_time, end_time, day, periodicity), tag(tag){};
    virtual ~activity(){};

    /// @brief 设置tag属性
    /// @param st 要添加的tag
    void set_tag(int st);

    /// @brief 消除tag属性
    /// @param et 要消除的tag
    void erase_tag(int et);
    virtual bool write_in(const std::string fname);
};
class tmpaffair : public activity
{
public:
    tmpaffair(int tag, position place, int start_time, int end_time, int day, int periodicity = 0)
        : activity(tag, place, start_time, end_time, day, periodicity){};
    virtual ~tmpaffair(){};

};


#endif // AFFAIR_H
