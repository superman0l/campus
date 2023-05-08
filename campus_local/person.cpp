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
    rootObject1.insert("place_id",-1);
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
    QString classid = rootObject2["class"].toString();
    int placeid=rootObject2["place_id"].toInt();

    if(rootObject2.value("isAdmin").toBool()){
        //鉴定为管理员用户
        admin_online = new Admin(nameValue.toString().toStdString(), user_id.toLongLong(),classid.toLongLong(),-1);
        user_online = NULL;
    }
    else{
        //鉴定为普通用户
        user_online =new User(nameValue.toString().toStdString(),user_id.toLongLong(),classid.toLongLong(),placeid);
        admin_online = NULL;
    }

    return true;
};
const std::vector<course> User::query(const QString& s, map* benbu, int tag) const{
    //tag=0 课程名字 tag=1老师名字
    std::vector<course> result;
    QJsonObject rootObject1;
    QString filepath = QString::number(id)+".json";
    if(!open_json(filepath,rootObject1))
        return std::vector<course>();
    QJsonValue classValue = rootObject1.value("class");
    QJsonObject rootObject2;
    if(!open_json(classValue.toString()+"_course.json",rootObject2))
        return std::vector<course>();
    QJsonValue arrayValue = rootObject2.value("courses");
    if (arrayValue.type() == QJsonValue::Array) {
        // 转换为QJsonArray类型
        QJsonArray courseArray = arrayValue.toArray();
        //遍历array
        for (int i = 0; i < courseArray.size(); i++) {
            QJsonValue jsoncourse = courseArray.at(i);
            if(jsoncourse.type()==QJsonValue::Object){
                QJsonObject courseObject = jsoncourse.toObject();

                QString cmpname;
                if(tag==0)
                    cmpname=courseObject.value("name").toString();
                else if(tag==1)
                    cmpname=courseObject.value("teacher").toString();
                if(cmpname.contains(s,Qt::CaseSensitive)){
                    result.push_back(
                        course(
                            courseObject.value("name").toString(),
                            benbu->idtopos[courseObject.value("destination_id").toInt()],
                            courseObject.value("starttime").toInt(),
                            courseObject.value("endtime").toInt(),
                            courseObject.value("weekday").toInt(),
                            1<<(courseObject.value("weekday").toInt()-1),
                            courseObject.value("startweek").toInt(),
                            courseObject.value("weekday").toInt(),
                            courseObject.value("teacher").toString(),
                            courseObject.value("classroom").toString(),
                            courseObject["platform"].toString(),
                            courseObject["url"].toString()
                        )
                    );
                }
            }
        }
    }
    return result;
}

const std::vector<QString> User::query_time(int day) const
{
    bool flag[7][14];
    std::vector<QString> result;

    QJsonObject rootObject1;//存储学生信息的json
    QString filepath = QString::number(id)+".json";
    if(!open_json(filepath,rootObject1))
        return std::vector<QString>();
    QJsonValue activityValue = rootObject1.value("activities");
    QJsonArray activityArray = activityValue.toArray();//activity队列
    for(int i=0;i<activityArray.size();i++){
        QJsonObject activityObject=activityArray[i].toObject();
        int day=activityObject["day"].toInt();int time=activityObject["time"].toInt();
        flag[day-1][time-8]=1;
    }
    QJsonArray coursearray = load_student_class_coursearray(QString::number(id));
    for(int i=0;i<coursearray.size();i++){
        QJsonObject course=coursearray.at(i).toObject();
        if(tim->get_week()<course["startweek"].toInt()||tim->get_week()>course["endweek"].toInt())continue;
        int day=course["weekday"].toInt();int sttime=course["starttime"].toInt();int edtime=course["endtime"].toInt();
        for(int j=sttime;j<=edtime;j++)flag[day-1][j-8]=1;
    }
    for(int i=0;i<7;i++){
        if(day!=0)i=day-1;
        for(int j=0;j<14;j++){
            if(!flag[i][j]){
                QString day=num_to_qstr(i+1);
                QString time=QString::number(j+8)+":00-"+QString::number(j+9)+":00";
                QString data=day+"  "+time;
                result.push_back(data);
            }
        }
        if(day!=0)break;
    }
    return result;
}

const std::vector<std::vector<tmpaffair> > User::query_tmpaffair() const
{
    std::vector<std::vector<tmpaffair>> result;
    std::vector<std::vector<tmpaffair>> count(15);
    QJsonObject user;
    open_json(QString::number(id)+".json",user);
    QJsonArray affairs=user["affairs"].toArray();
    for(int i=0;i<affairs.size();i++){
        QJsonObject affair=affairs.at(i).toObject();
        int time=affair["time"].toInt();
        tmpaffair ta=jsontotmpaffair(affair,*school_online);
        count[time-8].push_back(ta);
    }
    for(int i=0;i<15;i++){
        if(count[i].size()>1){
            result.push_back(count[i]);
        }
    }
    return result;
};
bool User::add_activity(const activity &a, int min) const{
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
        if((activityObject["day"].toInt()==0||a.day==activityObject["day"].toInt()||a.day==0)&&activityObject.value("time").toInt()==a.start){
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
        if(tim->get_week()<courseObject["startweek"].toInt()||tim->get_week()>courseObject["endweek"].toInt())continue;
        if((a.day==courseObject["weekday"].toInt()||a.day==0)
            && courseObject.value("starttime").toInt()<=a.start
            && courseObject.value("endtime").toInt()>=a.start){
            qDebug()<<"time error:activity conflicts course.";
            return false;
        }
    }
    //分别遍历activity和course以确定能够正常添加activity

    if(min!=0){
        rootObject2["alarm"]=alarm(true, a.day, a.start-1, 60-min, 1<<(a.day-1));
    }
    activityArray.append(rootObject2);
    rootObject1["activities"]=activityArray;
    if(!write_json(filepath,rootObject1))
        return false;
    return true;
}

bool User::del_activity(QString name, int day, int time) const
{
    QJsonObject rootObject1;//存储学生信息的json
    QString filepath = QString::number(id)+".json";
    if(!open_json(filepath,rootObject1))
        return false;
    QJsonValue activityValue = rootObject1.value("activities");
    QJsonArray activityArray = activityValue.toArray();//activity队列
    for(int i=0;i<activityArray.size();i++){
        if(activityArray.at(i).toObject()["name"].toString()==name&&activityArray.at(i).toObject()["day"].toInt()==day&&activityArray.at(i).toObject()["time"].toInt()==time){
            activityArray.removeAt(i);
            rootObject1["activities"]=activityArray;
            if(!write_json(filepath,rootObject1))
                return false;
            return true;
        }
    }
    return false;
}

bool User::add_tmpaffair(const tmpaffair &t) const
{
    QJsonObject tmp=tmpaffairtojson(t);
    QJsonArray course=load_student_class_coursearray(QString::number(user_online->get_id()));
    for(int i=0;i<course.size();i++){
        QJsonObject c=course.at(i).toObject();
        if(tim->get_week()<c["startweek"].toInt()||tim->get_week()>c["endweek"].toInt())continue;
        if((t.day==c["weekday"].toInt())
            && c.value("starttime").toInt()<=t.start
            && c.value("endtime").toInt()>=t.start){
            qDebug()<<"time error:tmpaffair conflicts course.";
            return false;
        }
    }

    QJsonObject user;
    open_json(QString::number(user_online->get_id())+".json", user);
    QJsonArray act=user["activities"].toArray();
    for(int i=0;i<act.size();i++){
        QJsonObject a=act.at(i).toObject();
        if(a["day"].toInt()==t.day&&a["time"].toInt()==t.start){
            qDebug()<<"time error:tempaffair conflicts activity.";
            return false;
        }
    }
    QJsonArray ta=user["affairs"].toArray();
    ta.append(tmp);
    user["affairs"]=ta;
    if(!write_json(QString::number(user_online->get_id())+".json",user))
        return false;
    return true;
}

bool User::del_tmpaffair(QString name, int time) const
{
    QJsonObject user;
    open_json(QString::number(user_online->get_id())+".json", user);
    QJsonArray ta=user["affairs"].toArray();
    for(int i=0;i<ta.size();i++){
        QJsonObject t=ta.at(i).toObject();
        if(t["name"].toString()==name&&t["time"].toInt()==time){
            ta.removeAt(i);
            user["affairs"]=ta;
            if(!write_json(QString::number(user_online->get_id())+".json", user))
                return false;
            return true;
        }
    }
    return false;
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
            break;
        }
    }
    rootObject1["activities"]=activityarray;
    if(!write_json(filepath,rootObject1))
        return false;
    return true;
}
bool User::set_clock_tmpaffair(const affair &a, bool enable)const{
    int day,hour,minute;
    day=a.day;hour=a.start;minute=0;
    QJsonObject rootObject1;//存储学生信息的json
    QString filepath = QString::number(id)+".json";
    if(!open_json(filepath,rootObject1))
        return false;
    QJsonArray tmpaffairarray=rootObject1["affairs"].toArray();
    for(int i=0;i<tmpaffairarray.size();i++){
        QJsonObject affair=tmpaffairarray.at(i).toObject();
        if(affair["name"].toString()==a.name&&affair["time"].toInt()==a.start){
            affair["alarm"]=alarm(enable, day, hour, minute, a.period);
            tmpaffairarray[i]=affair;
            break;
        }
    }
    rootObject1["affairs"]=tmpaffairarray;
    if(!write_json(filepath,rootObject1))
        return false;
    return true;
}
bool User::set_clock_course(const course &a, bool enable)const{
    int day,hour,minute;
    day=a.day;hour=a.start-1;minute=30;
    QJsonArray coursearray=load_student_class_coursearray(QString::number(id));
    for(int i=0;i<coursearray.size();i++){
        QJsonObject courseobject=coursearray[i].toObject();
        if(courseobject.value("name").toString()==a.name&&courseobject["weekday"].toInt()==a.day){
            courseobject["alarm"]=alarm(enable, day, hour, minute, a.period);
            coursearray[i]=courseobject;
            write_coursearray(QString::number(id),coursearray);
            return true;
        }
    }
    return false;
}
bool Admin::add_course(const course&cr,int64_t id)const{
    QJsonObject classObject;
    if(!open_json(QString::number(id)+"_course.json",classObject)){
        return false;
    }
    QJsonObject courseObject;
    courseObject=coursetojson(cr);
    QJsonArray coursearray=classObject["courses"].toArray();
    if(!course_check(cr,coursearray)){
        qDebug()<<"course conflict";
        return false;
    }
    coursearray.append(courseObject);
    classObject["courses"]=coursearray;
    if(!write_json(QString::number(id)+"_course.json",classObject)){
        return false;
    }
    return true;
}

bool Admin::erase_course(QString name, int day, int64_t id) const
{
    QJsonObject classObject;
    if(!open_json(QString::number(id)+"_course.json",classObject)){
        return false;
    }
    QJsonArray coursearray=classObject["courses"].toArray();
    for(int i=0;i<coursearray.size();i++){
        if(coursearray.at(i).toObject()["name"].toString()==name && coursearray.at(i).toObject()["weekday"].toInt()==day){
            coursearray.removeAt(i);
            break;
        }
    }
    classObject["courses"]=coursearray;
    if(!write_json(QString::number(id)+"_course.json",classObject)){
        return false;
    }
    return true;
}

bool Admin::update_course(const course &now, int64_t id, int old_day) const
{
    QJsonObject classObject;
    if(!open_json(QString::number(id)+"_course.json",classObject)){
        return false;
    }
    QJsonObject courseObject;
    courseObject=coursetojson(now);
    QJsonArray coursearray=classObject["courses"].toArray();
    for(int i=0;i<coursearray.size();i++){
        if(coursearray.at(i).toObject()["name"].toString()==now.name&&coursearray.at(i).toObject()["weekday"].toInt()==old_day){
            coursearray.removeAt(i);
            if(!course_check(now,coursearray)){
                qDebug()<<"course conflict";
                return false;
            }
            coursearray.append(coursetojson(now));
            break;
        }
    }
    classObject["courses"]=coursearray;
    if(!write_json(QString::number(id)+"_course.json",classObject)){
        return false;
    }
    return true;
}

