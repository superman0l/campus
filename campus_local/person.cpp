#include "person.h"
#include <QDebug>
#include <QJsonArray>
#include "affair.h"
#include "basic.h"
#include "online_data.h"

//extern const User* user_online;
//extern const Admin* admin_online;

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
bool login(QString user_id,QString user_pswd){
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
        admin_online = new Admin(nameValue.toString().toStdString(), user_id.toLongLong());
        user_online = NULL;
    }
    else{
        //鉴定为普通用户
        user_online =new User(nameValue.toString().toStdString() ,user_id.toLongLong());
        admin_online = NULL;
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
                            coursename,
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
    QJsonObject rootObject1;//存储学生信息的json
    QString filepath = QString::number(id)+".json";
    if(!open_json(filepath,rootObject1))
        return false;
    QJsonValue activityValue = rootObject1.value("activities");
    QJsonObject rootObject2=activitytojson(a);//activity
    QJsonArray activityArray = activityValue.toArray();//activity队列
    for(int i=0;i<activityArray.size();i++){
        QJsonObject activityObject=activityArray[i].toObject();
        if(activityObject.value("time").toInt()==a.start){
            qDebug()<<"time error:activity conflicts activity.";
            return false;
        }
    }

    QString filepath2 = rootObject1.value("class").toString()+"_course.json";
    QJsonObject rootObject3;//存储班级信息的json
    if(!open_json(filepath2,rootObject3))
        return false;
    QJsonValue courseValue = rootObject3.value("courses");
    QJsonArray courseArray = courseValue.toArray();//course队列
    for(int i=0;i<courseArray.size();i++){
        QJsonObject courseObject=courseArray[i].toObject();
        if(courseObject.value("starttime").toInt()<=a.start && courseObject.value("endtime").toInt()>=a.start){
            qDebug()<<"time error:activity conflicts course.";
            return false;
        }
    }
    //分别遍历activity和course以确定能够正常添加activity

    activityArray.append(rootObject2);
    rootObject1["activities"]=activityArray;
    if(!write_json(filepath,rootObject1))
        return false;
    return true;
}
bool User::set_clock_activity(const affair &a, int early_moment, bool enable)const{
    int day,hour,minute;
    day=a.day;
    if(early_moment==0){
        hour=a.start;
        minute=0;
    }
    else{
        hour=a.start-1;
        minute=60-early_moment;
    }
    QJsonObject rootObject1;//存储学生信息的json
    QString filepath = QString::number(id)+".json";
    if(!open_json(filepath,rootObject1))
        return false;
    QJsonArray activityarray=rootObject1["activities"].toArray();
    for(int i=0;i<activityarray.size();i++){
        QJsonObject activityobject=activityarray[i].toObject();
        if(activityobject.value("name").toString()==a.name){
            activityobject["alarm"]=alarm(enable, day, hour, minute, a.period);
            activityarray[i]=activityobject;
        }
    }
    rootObject1["activities"]=activityarray;
    if(!write_json(filepath,rootObject1))
        return false;
    return true;
}
bool User::set_clock_tmpaffair(const affair &a, bool enable)const{
    return true;
}
bool User::set_clock_course(const course &a, bool enable)const{
    int day,hour,minute;
    day=a.day;hour=a.start-1;minute=30;
    QJsonArray coursearray=load_student_class_coursearray(QString::number(id));
    for(int i=0;i<coursearray.size();i++){
        QJsonObject courseobject=coursearray[i].toObject();
        if(courseobject.value("name").toString()==a.name){
            courseobject["alarm"]=alarm(enable, day, hour, minute, a.period);
            coursearray[i]=courseobject;
            write_coursearray(QString::number(id),coursearray);
            return true;
        }
    }
    return false;
}
