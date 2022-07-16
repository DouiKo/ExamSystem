#include "userdata.h"

UserData::UserData(QString username, QString password, QString phone, QString school, QString classid, QString question, QString result)
{
    this->userName = username;
    this->passWord = password;
    this->phone = phone;
    this->school = school;
    this->classid = classid;
    this->question = question;
    this->result = result;
}
