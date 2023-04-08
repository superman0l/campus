#ifndef BASIC_H
#define BASIC_H

#include<bits/stdc++.h>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<QJsonObject>
#include<QJsonDocument>
#include <QJsonParseError>

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

#endif // BASIC_H
