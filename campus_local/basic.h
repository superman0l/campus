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

/// @brief 把course转化为json格式
/// @param 参数为需要转化的course类
/// @return 返回转化之后的QJsonObject
QJsonObject coursetojson(course c);

/// @brief 把json格式转化为course类
/// @param 参数为需要转化的json
/// @return 返回转化之后的course
course jsontocourse(QJsonObject rootObject);

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


#endif // BASIC_H
