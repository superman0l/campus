#include "person.h"
#include <QJsonArray>
#include "affair.h"
bool sign_up(QString rgs_id, QString rgs_pswd, QString rgs_name, QString rgs_class){
    QJsonObject rootObject;
    if(!open_json("id_pswd.json",rootObject))
        return false;
    rootObject.insert(rgs_id,rgs_pswd);
    if(!write_json("id_pswd.json",rootObject))//将账号密码数据写入json
        return false;

    QJsonObject rootObject1;
    rootObject1.insert("name",rgs_name);
    rootObject1.insert("id",rgs_id);
    rootObject1.insert("isAdmin", false);
    rootObject1.insert("class",rgs_class);
    if(!write_json(rgs_id+".json",rootObject1))//将个人相关数据写入json
        return false;
    return true;
};
bool login(QString user_id,QString user_pswd,const User*& user,const Admin*& adm){
    QJsonObject rootObject;
    if(!open_json("id_pswd.json",rootObject))
        return false;
    QJsonValue pswdValue = rootObject.value(user_id);
    if(pswdValue.toString()!=user_pswd){
        qDebug()<<"id or password error, please retry.";
        return false;
    }
    QJsonObject rootObject2;
    if(!open_json(user_id+".json",rootObject2)){
        qDebug()<<"account data exception, read failed.";
        return false;
    }
    QJsonValue nameValue = rootObject2.value("name");

    if(rootObject2.value("isAdmin").toBool()){
        //鉴定为管理员用户
        adm = new Admin(nameValue.toString().toStdString(), user_id.toLongLong());
        user = NULL;
    }
    else{
        //鉴定为普通用户
        user =new User(nameValue.toString().toStdString() ,user_id.toLongLong());
        adm = NULL;
    }

    return true;
};
const std::vector<course> User::query(const QString& s, map benbu) const{
    std::vector<course> result;
    QJsonObject rootObject1;
    QString filepath = QString::number(id)+".json";
    if(!open_json(filepath,rootObject1))
        return std::vector<course>();
    QJsonValue classValue = rootObject1.value("class");
    QJsonObject rootObject2;
    if(!open_json(classValue.toString()+"_course.json",rootObject2))
        return std::vector<course>();
    QJsonValue arrayValue = rootObject2.value("class");
    if (arrayValue.type() == QJsonValue::Array) {
        // 转换为QJsonArray类型
        QJsonArray courseArray = arrayValue.toArray();
        //遍历array
        for (int i = 0; i < courseArray.size(); i++) {
            QJsonValue jsoncourse = courseArray.at(i);
            if(jsoncourse.type()==QJsonValue::Object){
                QJsonObject courseObject = jsoncourse.toObject();
                QJsonValue nameValue = courseObject.value("name");
                QString coursename = nameValue.toString();
                if(coursename.contains(s,Qt::CaseSensitive)){
                    result.push_back(
                        course(
                            benbu.idtopos[courseObject.value("destination_id").toInt()],
                            courseObject.value("starttime").toInt(),
                            courseObject.value("endtime").toInt(),
                            courseObject.value("weekday").toInt(),
                            1<<(courseObject.value("weekday").toInt()-1),
                            courseObject.value("startweek").toInt(),
                            courseObject.value("weekday").toInt()
                        )
                    );
                }
            }
        }
    }
    return result;
};
bool User::add_activity(const activity &a) const{
    //int tag, position place, int start_time, int end_time, int day, int periodicity = 0
    QJsonObject rootObject1;
    QString filepath = QString::number(id)+".json";
    if(!open_json(filepath,rootObject1))
        return false;
    QJsonValue activityValue = rootObject1.value("activities");
    QJsonObject rootObject2;//activity
    rootObject2.insert("alarm",empty_alarm());
    rootObject2.insert("day",a.day);
    rootObject2.insert("destination_id",a.day);
    rootObject2.insert("day",a.day);
    //"day": 7,
    //            "destination_id": 4,
    //            "name": "去物美买东西",
    //            "tag": 1,
    //            "time": 16


    return true;
}
