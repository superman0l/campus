#ifndef PERSON_H
#define PERSON_H

#include <bits/stdc++.h>
#include "affair.h"
#include<QFile>
#include<QJsonObject>
#include<QString>
//#include"basic.h"

class User
{
private:
    std::string name;
    int place_id=-1;
    int64_t id;
    int64_t classid;

public:
    User(const std::string &username, int64_t id, int64_t classid, int place_id) : name(username), id(id), classid(classid), place_id(place_id){}
    virtual ~User() {}
    const std::string get_name() const
    {
        return name;
    }
    int64_t get_id() const
    {
        return id;
    }
    int get_place_id()const{
        return place_id;
    }
    int64_t get_classid() const
    {
        return classid;
    }

    //更改place_id信息
    void set_place_id(int plcid){
        place_id = plcid;
    }
    /// @brief 判断当前用户是否是管理员
    /// @return true表示当前用户是管理员,false则相反
    virtual bool is_admin() const
    {
        return false;
    }

    /// @brief 课程查询功能
    /// @param s 课程名
    /// @return 查询结果
    const std::vector<course> query(const QString& s, map* school, int tag)const;

    /// @brief 临时事务查询功能
    /// @param begin_time 查询的起始时间
    /// @param end_time 查询的最终时间
    /// @param ntag 查询的tag
    /// @return 查询结果
    const std::vector<tmpaffair> query(int begin_time,int end_time,int ntag=0)const;

    /// @brief 查询空闲时间功能
    /// @param day 查询的时间
    /// @return 查询时间结果，用字符串表示，“星期三 18:00-19:00”
    const std::vector<QString> query_time(int day)const;

    /// @brief 查询同一时间事务功能
    /// @return 查询时间结果，用字符串表示，“星期三 18:00-19:00”
    const std::vector<std::vector<tmpaffair>> query_tmpaffair()const;

    /// @brief 添加活动功能
    /// @param a 添加的活动
    /// @param min 添加事务用闹钟
    /// @return true表示活动添加成功,false表示由于冲突活动添加失败
    bool add_activity(const activity &a, int min)const;

    /// @brief 删除活动功能
    /// @param name 删除的活动名
    /// @param day 删除的活动日
    /// @param time 删除的活动时间
    /// @return true表示活动删除成功,false表示未找到活动删除失败
    bool del_activity(QString name,int day,int time)const;

    /// @brief 添加临时事务功能
    /// @param a 添加的临时事务
    /// @return true表示事务添加成功,false表示添加失败
    bool add_tmpaffair(const tmpaffair &t)const;

    /// @brief 删除临时事务功能
    /// @param name 删除的名
    /// @param time 删除的时间
    /// @return true表示活动事务成功,false表示未找到事务删除失败
    bool del_tmpaffair(QString name,int time)const;

    /// @brief 添加闹钟功能(activity类)
    /// @param a 为a事务添加闹钟
    /// @param early_moment 决定提前响铃多久
    /// @param enable 闹钟开关
    /// @return true表示设置闹钟成功，反之失败
    bool set_clock_activity(const affair &a, int early_moment, bool enable)const;

    /// @brief 添加闹钟功能(tmpaffair类)
    /// @param a 为a事务添加闹钟
    /// @param enable 闹钟开关
    /// @return true表示设置闹钟成功，反之失败
    bool set_clock_tmpaffair(const affair &a, bool enable)const;

    /// @brief 添加闹钟功能(course类)
    /// @param a 为a事务添加闹钟
    /// @param enable 闹钟开关
    /// @return true表示设置闹钟成功，反之失败
    bool set_clock_course(const course &a, bool enable)const;
};
class Admin : public User
{
public:
    Admin(const std::string &username, int64_t id, int64_t classid,int place_id) : User(username, id, classid, place_id) {}
    virtual ~Admin() {}

    /// @brief 判断当前用户是否是管理员
    /// @return true表示当前用户是管理员,false表示添加失败
    virtual bool is_admin()const override
    {
        return true;
    }

    /// @brief 管理员添加课程功能
    /// @param cr 要添加的课程
    /// @param id 班级编号
    /// @return true表示添加成功,false表示添加失败
    bool add_course(const course&cr,int64_t id)const;

    /// @brief 管理员删除课程功能
    /// @param cr 要删除的课程(该参数仅供索引使用)
    /// @param id 班级编号
    /// @return true表示删除成功,false表示删除失败
    bool erase_course(QString name, int day, int64_t id)const;

    /// @brief 管理员修改课程功能
    /// @param old 要修改的旧的课程(该参数仅供索引使用)
    /// @param now 新课程
    /// @param id 班级编号
    /// @return true表示修改成功,false表示修改失败
    bool update_course(const course&now, int64_t id, int old_day)const;

};
/// @brief 账号登录功能
/// @param user_id  登录使用id
/// @param user_pswd 登录使用密码
/// @param user 一个用户指针的引用，登录失败或登录上管理员用户为NULL值
/// @param adm  一个管理员指针的引用，登录失败或登陆上普通用户为NULL值
/// @return 若登录成功返回true,若登录失败返回flase
bool login(QString user_id,QString user_pswd);

/// @brief 账号注册功能
/// @param rgs_id 注册者使用的学号/id
/// @param rgs_pswd 注册者使用的密码
/// @param rgs_name 注册者姓名
/// @param rgs_class 注册者所在班级
/// @return true表示注册成功,false表示注册失败
bool sign_up(QString rgs_id, QString rgs_pswd, QString rgs_name, QString rgs_class);
class Group{
    public:
        std::vector<User> ug;
        int64_t id;

    /// @brief 初始化班级人员
    /// @param fname 存储文件名称
    /// @param id 班级编号
    Group(QString fname,int64_t id):id(id){};
};


#endif // PERSON_H
