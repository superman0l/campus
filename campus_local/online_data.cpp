
#include "person.h"
#include "map.h"
#include "timer.h"
#include"log.h"
#include<QSoundEffect>

const User* user_online;//保存用户登录信息

const Admin* admin_online;//保存管理员登录信息

map* school_online;

timer* tim;

QSoundEffect bell;

Log user_log;
