#include "usermainscn.h"

#include <QApplication>
#include <qjsonobject.h>
#include"extcpsocket.h"
#include"userloginscn.h"
#include"QSqlQuery"
#include"qsqldatabase.h"

ExTcpSocket *tcpClient;
QString currUserName;
QSqlQuery outQuery;
QSqlQuery inQuery;

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::ApplicationAttribute::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    tcpClient = new ExTcpSocket("127.0.0.1",6666);
    tcpClient->sendMsg("123",QJsonObject());
    currUserName = "";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("local.db");
    if(db.open()){
        qDebug()<<"database open success";
        outQuery = QSqlQuery(db);
        inQuery = QSqlQuery(db);
    }

    UserLoginScn w;
    w.show();
    return a.exec();
}
