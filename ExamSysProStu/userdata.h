#ifndef USERDATA_H
#define USERDATA_H
#include"qobject.h"

class UserData
{
public:
    UserData(QString username, QString password, QString phone, QString school, QString classid, QString question, QString result);
public:
    QString userName;
    QString passWord;
    QString phone;
    QString school;
    QString classid;
    QString question;
    QString result;
};

#endif // USERDATA_H
