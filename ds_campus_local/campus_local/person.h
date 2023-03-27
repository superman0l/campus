#ifndef PERSON_H
#define PERSON_H

#include <bits/stdc++.h>
#include "affair.h"
#include<QFile>
#include<QJsonObject>
#include"basic.h"

class User
{
private:
    std::string name;
    int64_t id;

public:
    User(const std::string &username, int64_t id) : name(username), id(id) {}
    virtual ~User() {}
    const std::string get_name()
    {
        return name;
    }
    int64_t get_id()
    {
        return id;
    }

    /// @brief 判断当前用户是否是管理员
    /// @return true表示当前用户是管理员,false则相反
    virtual bool is_admin()
    {
        return false;
    }

    /// @brief 课程查询功能
    /// @param s 课程名
    /// @return 查询结果
    const std::vector<course> query(const std::string s);

    /// @brief 临时事务查询功能
    /// @param begin_time 查询的起始时间
    /// @param end_time 查询的最终时间
    /// @param ntag 查询的tag
    /// @return 查询结果
    const std::vector<tmpaffair> query(int begin_time,int end_time,int ntag=0);

    /// @brief 添加活动,临时事务功能
    /// @param a 添加的活动或临时事务
    /// @return true表示活动添加成功,false表示由于冲突活动添加失败
    bool add_activity(const activity &a);

    /// @brief 添加闹钟功能
    /// @param a 为a事务添加闹钟
    /// @param periodicity 闹钟的周期
    void add_clock(const affair &a, int periodicity=0);

};
class Admin : public User
{
public:
    Admin(const std::string &username, int64_t id) : User(username, id) {}
    virtual ~Admin() {}

    /// @brief 判断当前用户是否是管理员
    /// @return true表示当前用户是管理员,false表示添加失败
    virtual bool is_admin() override
    {
        return true;
    }

    /// @brief 管理员添加课程功能
    /// @param cr 要添加的课程
    /// @param id 班级编号
    /// @return true表示添加成功,false表示添加失败
    bool add_course(const course&cr,int64_t id);

    /// @brief 管理员删除课程功能
    /// @param cr 要删除的课程(该参数仅供索引使用)
    /// @param id 班级编号
    /// @return true表示删除成功,false表示删除失败
    bool erase_course(const course&cr,int64_t id);

    /// @brief 管理员修改课程功能
    /// @param old 要修改的旧的课程(该参数仅供索引使用)
    /// @param now 新课程
    /// @param id 班级编号
    /// @return true表示修改成功,false表示修改失败
    bool update_course(const course&old,const course&now,int64_t id);


};
const User *login();
bool sign_up(QString rgs_id,QString rgs_pswd){
    QJsonObject rootObject;
    if(!open_json("id_pswd.json",rootObject))
        return false;
    rootObject.insert(rgs_id,rgs_pswd);
    if(!write_json("id_pswd.json",rootObject))
        return false;
    return true;
};
class Group{
    public:
        std::vector<User> ug;
        int64_t id;

    /// @brief 初始化班级人员
    /// @param fname 存储文件名称
    /// @param id 班级编号
    Group(std::string fname,int64_t id):id(id){};
};

#endif // PERSON_H
