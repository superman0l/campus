#ifndef BASIC_H
#define BASIC_H

#include<bits/stdc++.h>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<QJsonObject>
#include<QJsonDocument>
#include <QJsonParseError>

bool open_json(QString jsonname,QJsonObject& objectname){
    QFile file("../Json/"+jsonname);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "readfile can't open error!";
        return false;
    }

    // 读取文件
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);		// 设置读取编码是UTF8
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
