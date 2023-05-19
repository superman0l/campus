#ifndef LOG_H
#define LOG_H
#include<QFile>
#include<QString>
#include"person.h"
class Log
{
private:
    bool user_is_admin;//日志对应用户是否为管理员
    QFile stream;
public:
    Log();
    void init(const User* user);
    /// @brief write 向日志写入信息
    /// @param s 信息
    void write(QString s);
    ~Log();
};
#define logvar user_log
#define log_action(x)\
    do{\
    QString __log_s=QString("[ACTION] %1 %2 %3\n").arg(tim->get_DateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(QString::fromStdString(user_online->get_name())).arg(x);\
    logvar.write(__log_s);\
    }while(false);

#define log_event(x)\
    do{\
    QString __log_s=QString("[EVENT] %1 %2 %3\n").arg(tim->get_DateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(QString::fromStdString(user_online->get_name())).arg(x);\
    logvar.write(__log_s);\
    }while(false);
#endif // LOG_H
