#ifndef CLIENTMSG_H
#define CLIENTMSG_H

#include <QObject>
#include"qdatastream.h"
#include"qtcpsocket.h"
#include"qsqlquery.h"
#include"qjsondocument.h"
#include"qjsonarray.h"
#include"qjsonobject.h"
#include"qfile.h"
#include"qsqlerror.h"

class ClientMsg : public QObject
{
    Q_OBJECT
public:
    explicit ClientMsg(QByteArray readData,QTcpSocket *socket,QObject *parent = nullptr);

private:
    bool sendMsg(QString type,QJsonObject data,QTcpSocket *socket);
    bool sendMsg(QString type,QJsonArray array,QTcpSocket *socket);

private: //根据消息头选择具体执行操作
    QJsonObject studentLogin(QJsonObject jsonObject);
    QJsonObject teacherLogin(QJsonObject jsonObject);
    QJsonObject studentBackup(QJsonObject jsonObject);
    QJsonObject teacherBackup(QJsonObject jsonObject);
    QJsonObject studentReg(QJsonObject jsonObject);
    QJsonObject teacherReg(QJsonObject jsonObject);
    QJsonObject createQuesJson(QString title,QString type,QString c1,QString c2,QString c3,QString c4,QString c5,QString c6,QString result,QString level,QString score,QString teacher);

private: //处理接收到的消息类型（消息头）
    void receiveLoginMsg(QByteArray data,QTcpSocket *socket);
    void receiveRegMsg(QByteArray data,QTcpSocket *socket);
    void receiveAddQuestionMsg(QByteArray data);
    void receiveGetQuestionMsg(QByteArray data,QTcpSocket *socket);
    void receiveRemoveQuestionMsg(QByteArray data);
    void receiveAddNewExam(QByteArray data);
    void receiveAddRandExam(QByteArray data,QTcpSocket *socket);
    void receiveAddGetExamList(QByteArray data,QTcpSocket *socket);
    void receiveRemoveExamList(QByteArray data);
    void receiveGetExam(QByteArray data,QTcpSocket *socket);
    void receiveGetFreeExam(QByteArray data,QTcpSocket *socket);
    void receiveCommitExam(QByteArray data);
    void receiveSearchExam(QByteArray data,QTcpSocket *socket);
    void receiveBillExamMsg(QByteArray data);
    void receiveGetExamLogMsg(QByteArray data,QTcpSocket *socket);
    void receiveResetUserMsg(QByteArray data);
    void receiveUserBackupMsg(QByteArray data,QTcpSocket *socket);
private:
    void insertExamToDataBaseAndSaveAsJson(QJsonArray jsonArray);
    QJsonArray createRandQuesJson(int singleNumner,int muchNumber,int tofNumber,QString level,QString examName,QString teacher,QString isRand,QTcpSocket *socket);
    void onAddSingleQues(QJsonObject jsonObject);
    void onAddMuchQues(QJsonObject jsonObject);
    void onAddTofQues(QJsonObject jsonObject);
private:   //类字段

signals:

};

#endif // CLIENTMSG_H
