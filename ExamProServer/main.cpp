#include "mainwindow.h"

#include <QApplication>
#include"qsqldatabase.h"
#include"qsqlquery.h"

QSqlQuery outQuery;
QSqlQuery inQuery;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("dk123456");
    db.setDatabaseName("exam");

    if(db.open()){
        outQuery = QSqlQuery(db);
        inQuery = QSqlQuery(db);
        outQuery.exec("use exam");
        inQuery.exec("use exam");
    }

    MainWindow w;
    w.show();
    return a.exec();
}
