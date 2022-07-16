#ifndef EXTCPSOCKET_H
#define EXTCPSOCKET_H

#include <QObject>
#include"qtcpsocket.h"

class ExTcpSocket : public QObject
{
    Q_OBJECT
public:
    ExTcpSocket(QString ip,quint16 port);
    QTcpSocket* getInstance();
    bool sendMsg(QString type,QJsonObject data);
private:
    QTcpSocket *tcpClient;
private slots:
    void onReceiveData();
signals:
    void receiveData(QByteArray);
};

#endif // EXTCPSOCKET_H
