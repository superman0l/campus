#include "log.h"
#include"person.h"
Log::Log()
{

}
Log::~Log()
{
    if(stream.isOpen())
    {
        stream.close();
    }

}
void Log::init(const User*user)
{
    if(user->is_admin())
    {
        this->user_is_admin=true;
    }else
    {
        this->user_is_admin=false;
    }
    stream.setFileName(QString("../")+QString::number(user->get_id())+"_log.txt");
    stream.open(QIODevice::WriteOnly|QIODevice::Append);
}
void Log::write(QString s)
{
    stream.write(s.toStdString().c_str());
}
