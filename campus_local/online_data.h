
#ifndef ONLINE_DATA_H
#define ONLINE_DATA_H
#include "person.h"
#include "map.h"
#include "timer.h"

extern const User* user_online;//保存用户登录信息

extern const Admin* admin_online;//保存管理员登录信息

extern map* school_online;

extern timer* tim;
#endif // ONLINE_DATA_H
