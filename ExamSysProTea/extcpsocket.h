#ifndef EXTCPSOCKET_H
#define EXTCPSOCKET_H

#include <QObject>
#include"qtcpsocket.h"
#include"qjsonarray.h"
#include"qjsonobject.h"

class ExTcpSocket : public QObject
{
    Q_OBJECT
public:
    ExTcpSocket(QString ip,quint16 port);
    QTcpSocket* getInstance();
    bool sendMsg(QString type,QJsonObject data);
    bool sendMsg(QString type,QJsonArray array);
private:
    QTcpSocket *tcpClient;
private slots:
    void onReceiveData();
    void onError(QAbstractSocket::SocketError socketError);
signals:
    void receiveData(QByteArray);
};

#endif // EXTCPSOCKET_H
