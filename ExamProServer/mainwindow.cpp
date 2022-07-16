#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"qjsondocument.h"
#include"qjsonvalue.h"
#include"qjsonobject.h"
#include"qsqlquery.h"
#include"qsqlerror.h"

#include <QJsonArray>
#include <qthread.h>
#include"clientmsg.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);
    connect(tcpServer,&QTcpServer::newConnection,this,&MainWindow::onNewConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    readData += socket->readAll();   //读取一次套接字缓冲区中全部内容并添加至byte数组（合包）
    QByteArray endByte = "!End";      //终止符号byte
    if(readData.contains(endByte)){   //读取到endByte代表全部读取完毕，执行逻辑
        ClientMsg clientMsg(readData,socket);      //接收到客户端消息创建msg对象,所有执行逻辑均在子线程对象中执行，执行完成后自动析构释放
        readData.clear();  //执行完毕后清空byte数组
        qDebug()<<"读取结束";
    }
    //单独处理每个消息子对象
}

void MainWindow::onNewConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    if(!tcpSocket){
        return;
    }
    connect(tcpSocket,&QTcpSocket::readyRead,this,&MainWindow::onReadyRead);
    connect(tcpSocket,&QTcpSocket::disconnected,tcpSocket,&QTcpSocket::deleteLater);
    qDebug()<<"new user login";
}

void MainWindow::on_btnStartServer_clicked()
{
    tcpServer->listen(QHostAddress("127.0.0.1"),6666);
    qDebug()<<"server start,on listening";
}

void MainWindow::on_btnStopServer_clicked()
{
    tcpServer->close();
    qDebug()<<"stop listening";
}

