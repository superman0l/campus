#include "person.h"
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
