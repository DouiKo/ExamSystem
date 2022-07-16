#include "extcpsocket.h"

#include <QAbstractSocket>
#include <QDataStream>
#include <QTimerEvent>
#include"qjsonobject.h"

extern QString currUserName;

ExTcpSocket::ExTcpSocket(QString ip,quint16 port)
{
    tcpClient = new QTcpSocket(this);
    tcpClient->connectToHost(ip,port);
    connect(tcpClient,&QTcpSocket::readyRead,this,&ExTcpSocket::onReceiveData);
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
}

QTcpSocket *ExTcpSocket::getInstance()
{
    return tcpClient;
}

bool ExTcpSocket::sendMsg(QString type, QJsonObject data)
{
    if(tcpClient->waitForConnected(100)){
        QByteArray byteArray;
        QByteArray endByte = "!End";
        QDataStream out(&byteArray,QIODevice::ReadWrite);
        out << type << currUserName << data <<endByte;
        tcpClient->write(byteArray);
        return true;
    }else{
        return false;
    }
}

bool ExTcpSocket::sendMsg(QString type, QJsonArray array)
{
    if(tcpClient->waitForConnected(100)){
        QByteArray byteArray;
        QByteArray endByte = "!End";
        QDataStream out(&byteArray,QIODevice::ReadWrite);
        out << type << currUserName << array <<endByte;
        tcpClient->write(byteArray);
        return true;
    }else{
        return false;
    }
}

void ExTcpSocket::onReceiveData()
{
    QByteArray data = tcpClient->readAll();
    emit receiveData(data);
}

void ExTcpSocket::onError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<socketError;
}
