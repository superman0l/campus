#ifndef BASIC_H
#define BASIC_H

#include<bits/stdc++.h>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<QJsonObject>
#include<QJsonDocument>
#include <QJsonParseError>
#include "affair.h"
//#include "person.h"
#include <QJsonArray>

/// @brief 打开json文件并读取为QJsonObject
/// @param jsonname 要打开的json文件名
/// @param objectname 要使用的QJsonObject名字
/// @return true表示读取成功,false表示读取失败
bool open_json(QString jsonname,QJsonObject& objectname);

/// @brief 将QJsonObject写入json文件中
/// @param jsonname 要写入的json文件名
/// @param objectname 要使用的QJsonObject名字
/// @return true表示写入成功,false表示写入失败
bool write_json(QString jsonname,QJsonObject objectname);

/// @brief 创建一个空的闹钟Json并返回，用于新建任务
/// @return 返回QJsonObject表示空闹钟
QJsonObject empty_alarm();

/// @brief 闹钟参数传入json
/// @param enable表示闹钟开关
/// @param day表示闹钟weekday
/// @param hour
/// @param minute
/// @param periodicity传入affair.period
/// @return 返回QJsonObject表示闹钟
QJsonObject alarm(bool enable, int day, int hour, int minute, int periodicity);

/// @brief 把course转化为json格式
/// @param 参数为需要转化的course类
/// @return 返回转化之后的QJsonObject
QJsonObject coursetojson(course c);

/// @brief 把json格式转化为course类
/// @param 参数为需要转化的json
/// @return 返回转化之后的course
course jsontocourse(QJsonObject rootObject, map* school);

/// @brief 把activity转化为json格式
/// @param 参数为需要转化的activity类
/// @return 返回转化之后的QJsonObject
QJsonObject activitytojson(activity a);

/// @brief 把json转化为activity
/// @param 参数为需要转化的json
/// @return 返回转化之后的activity
activity jsontoactivity(QJsonObject rootObject);

/// @brief 把json转化为tmpaffair
/// @param 参数为需要转化的json
/// @return 返回转化之后的tmpaffair
tmpaffair jsontotmpaffair(QJsonObject rootObject);

/// @brief 把tmpaffair转化为json格式
/// @param 参数为需要转化的tmpaffair类
/// @return 返回转化之后的QJsonObject
QJsonObject tmpaffairtojson(tmpaffair t);

/// @brief 根据学生学号读出课表
/// @param id为学生学号
/// @return 返回读出的的QJsonArray
QJsonArray load_student_class_coursearray(QString id);

/// @brief 写入更改的课程数据
/// @param id为学生学号
/// @param coursearray为更改后的课程array
void write_coursearray(QString id,QJsonArray coursearray);

/// @brief 根据课程名称读取json返回object
/// @param id为学生学号
/// @param name为课程名称
/// @return 返回读出的的QJsonObject
QJsonObject load_course_json(QString id, QString name);

/// @brief 根据数字返回星期几
/// @param num为星期天数
/// @return 返回string星期几
QString num_to_qstr(int num);

#endif // BASIC_H
