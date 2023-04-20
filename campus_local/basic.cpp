#include "basic.h"
#include "affair.h"
#include "map.h"
#include <QJsonObject>
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
QJsonObject empty_alarm(){
    QJsonObject rootObject;
    rootObject.insert("enable",false);
    rootObject.insert("day",0);
    rootObject.insert("hour",0);
    rootObject.insert("minite",0);
    rootObject.insert("periodicity",0);
    return rootObject;
}
QJsonObject alarm(bool enable, int day, int hour, int minute, int periodicity){
    QJsonObject rootObject;
    rootObject.insert("enable",enable);
    rootObject.insert("day",day);
    rootObject.insert("hour",hour);
    rootObject.insert("minite",minute);
    rootObject.insert("periodicity",periodicity);
    return rootObject;
}
QJsonObject coursetojson(course c){
    QJsonObject rootObject;
    rootObject.insert("alarm",empty_alarm());
    rootObject.insert("destination_id",c.place.id);
    rootObject.insert("name",c.name);
    rootObject.insert("teacher",c.teacher);
    rootObject.insert("starttime",c.start);
    rootObject.insert("endtime",c.end);
    rootObject.insert("startweek",c.start_week);
    rootObject.insert("endweek",c.end_week);
    rootObject.insert("weekday",c.day);
    rootObject.insert("classroom",c.classroom);
    return rootObject;
}
course jsontocourse(QJsonObject rootObject, map* school){
    QJsonValue idValue=rootObject.value("destination_id");
    QJsonValue nameValue=rootObject.value("name");
    QJsonValue teacherValue=rootObject.value("teacher");
    QJsonValue startValue=rootObject.value("starttime");
    QJsonValue endValue=rootObject.value("endtime");
    QJsonValue startweekValue=rootObject.value("startweek");
    QJsonValue endweekValue=rootObject.value("endweek");
    QJsonValue weekdayValue=rootObject.value("weekday");
    QJsonValue classroomValue=rootObject.value("classroom");
    return course(
        nameValue.toString(),
        school->idtopos[idValue.toInt()],
        startValue.toInt(),
        endValue.toInt(),
        weekdayValue.toInt(),
        1<<(weekdayValue.toInt()-1),
        startweekValue.toInt(),
        endweekValue.toInt(),
        teacherValue.toString(),
        classroomValue.toString()
    );
}
QJsonObject activitytojson(activity a){
    QJsonObject rootObject;
    rootObject.insert("alarm",empty_alarm());
    rootObject.insert("destination_id",a.place.id);
    rootObject.insert("name",a.name);
    rootObject.insert("tag",a.tag);
    rootObject.insert("time",a.start);
    rootObject.insert("day",a.day);
    return rootObject;
}
activity jsontoactivity(QJsonObject rootObject, map school){
    QJsonValue idValue=rootObject.value("destination_id");
    QJsonValue nameValue=rootObject.value("name");
    QJsonValue tagValue=rootObject.value("tag");
    QJsonValue startValue=rootObject.value("time");
    QJsonValue weekdayValue=rootObject.value("day");
    return activity(
        nameValue.toString(),
        tagValue.toInt(),
        school.idtopos[idValue.toInt()],
        startValue.toInt(),
        startValue.toInt()+1,
        weekdayValue.toInt(),
        1<<(weekdayValue.toInt()-1)
    );
}
QJsonObject tmpaffairtojson(tmpaffair t){
    QJsonObject rootObject;
    rootObject.insert("alarm",empty_alarm());
    rootObject.insert("destination_id",t.place.id);
    rootObject.insert("name",t.name);
    rootObject.insert("tag",t.tag);
    rootObject.insert("time",t.start);
    rootObject.insert("day",t.day);
    return rootObject;
}
activity jsontotmpaffair(QJsonObject rootObject, map school){
    QJsonValue idValue=rootObject.value("destination_id");
    QJsonValue nameValue=rootObject.value("name");
    QJsonValue tagValue=rootObject.value("tag");
    QJsonValue startValue=rootObject.value("time");
    QJsonValue weekdayValue=rootObject.value("day");
    return activity(
        nameValue.toString(),
        tagValue.toInt(),
        school.idtopos[idValue.toInt()],
        startValue.toInt(),
        startValue.toInt()+1,
        weekdayValue.toInt(),
        1<<(weekdayValue.toInt()-1)
    );
}
QJsonArray load_student_class_coursearray(QString id){
    QJsonObject studentObject;
    open_json(id+".json", studentObject);
    QString classid = studentObject["class"].toString();
    QJsonObject classObject;
    open_json(classid+"_course.json", classObject);
    QJsonArray courseArray=classObject["courses"].toArray();
    return courseArray;
}
void write_coursearray(QString id,QJsonArray coursearray){
    QJsonObject studentObject;
    open_json(id+".json", studentObject);
    QString classid = studentObject["class"].toString();
    QJsonObject classObject;
    open_json(classid+"_course.json", classObject);
    classObject["courses"]=coursearray;
    write_json(classid+"_course.json", classObject);
}
QJsonObject load_course_json(QString id, QString name){
    QJsonArray coursearray = load_student_class_coursearray(id);
    for(int i=0;i<coursearray.size();i++){
        QJsonObject course=coursearray.at(i).toObject();
        QString cname=course["name"].toString();
        if(cname==name){
            return course;
        }
    }
    return QJsonObject();
}
QString num_to_qstr(int num){
    switch (num) {
    case 1:
        return "星期一";
    case 2:
        return "星期二";
    case 3:
        return "星期三";
    case 4:
        return "星期四";
    case 5:
        return "星期五";
    case 6:
        return "星期六";
    case 7:
        return "星期日";
    default:
        return "错误输入";
    }
    return "错误输入";
}
int qstr_to_num(QString day){
    if(day=="星期一")
        return 1;
    else if(day=="星期二")
        return 2;
    else if(day=="星期三")
        return 3;
    else if(day=="星期四")
        return 4;
    else if(day=="星期五")
        return 5;
    else if(day=="星期六")
        return 6;
    else if(day=="星期日")
        return 7;
    else
        return 0;
}
void qstr_to_time(QString time, int& st, int& ed){
    QString start,end;
    int f,c=0;
    for(int i=0;i<time.length();i++){
        if(time[i]==':'){
            if(c==0){
                start=time.mid(0,i);
                c=1;
            }
            else if(c==1){
                end=time.mid(f,i-f);
                break;
            }
        }
        else if(time[i]=='-'){
            f=i+1;
        }
    }
    st=start.toInt();
    ed=end.toInt();
}
