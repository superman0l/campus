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
bool open_json(QString jsonname,QJsonObject& objectname){
    QFile file("../Json/"+jsonname);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "readfile can't open error!";
        return false;
    }

    // 读取文件
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    QString str = stream.readAll();
    file.close();

    // JSON解析期间报告错误。
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
    // 判断解析失败
    if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
        qDebug() << "Json error！" << jsonError.error;
        return false;
    }
    objectname=doc.object();
    return true;
}

/// @brief 将QJsonObject写入json文件中
/// @param jsonname 要写入的json文件名
/// @param objectname 要使用的QJsonObject名字
/// @return true表示写入成功,false表示写入失败
bool write_json(QString jsonname,QJsonObject objectname){
    QJsonDocument doc;
    doc.setObject(objectname);
    QFile file("../Json/"+jsonname);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "writefile can't open error!";
        return false;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << doc.toJson();
    file.close();
    return true;
}

#endif // BASIC_H
