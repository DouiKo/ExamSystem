#include "userloginscn.h"
#include "ui_userloginscn.h"
#include"qmessagebox.h"
#include"qjsondocument.h"
#include"qjsonobject.h"
#include"qjsonvalue.h"
#include"userregdialog.h"
#include"qdebug.h"
#include"extcpsocket.h"
#include"userregdialog.h"
#include"userdata.h"
#include <QBitmap>
#include <QPainter>
#include"userbackupdialog.h"

extern ExTcpSocket *tcpClient;

UserLoginScn::UserLoginScn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserLoginScn)
{
    ui->setupUi(this);
    setRoundWidget();
    connect(tcpClient,&ExTcpSocket::receiveData,this,&UserLoginScn::receiveMsg);
}

UserLoginScn::~UserLoginScn()
{
    delete ui;
}

void UserLoginScn::receiveMsg(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType;
    in >> msgType;
    qDebug()<<msgType;
    if(msgType == "logindone"){
        QJsonObject jsonObject;
        in >> jsonObject;
        if(jsonObject.value("error").toString() == "none"){
            QString username = jsonObject.value("username").toString();
            QString password = jsonObject.value("password").toString();
            QString phone = jsonObject.value("phone").toString();
            QString school = jsonObject.value("school").toString();
            QString classid = jsonObject.value("classid").toString();
            QString question = jsonObject.value("question").toString();
            QString result = jsonObject.value("result").toString();
            UserData user(username,password,phone,school,classid,question,result);
            mainScn = new UserMainScn(user);
            mainScn->show();
            this->close();
        }
        if(jsonObject.value("error").toString() == "nouser"){
            QMessageBox::information(nullptr,"登录","用户不存在，请检查用户名",nullptr,nullptr);
        }
        if(jsonObject.value("error").toString() == "passerror"){
            QMessageBox::information(nullptr,"登录","用户名或密码错误",nullptr,nullptr);
        }
    }
}

void UserLoginScn::on_btnLogin_clicked()
{
    QString userName = ui->leUser->text();
    QString passWord = ui->lePass->text();
    if(userName != "" && passWord != ""){
        QString username = "test";
        QString password = "123";
        QString phone = "123";
        QString school = "测试";
        QString classid = "1";
        QString question = "1";
        QString result = "1";
        UserData user(username,password,phone,school,classid,question,result);
        mainScn = new UserMainScn(user);
        mainScn->show();
        this->close();
//        QJsonObject jsonObject;
//        jsonObject.insert("role","teacher");
//        jsonObject.insert("username",userName);
//        jsonObject.insert("password",passWord);
//        try {
//            tcpClient->sendMsg("login",jsonObject);
//        } catch (...) {
//            //...
//        }
    }else{
        QMessageBox::information(nullptr,"登录","用户名或密码不能为空",nullptr,nullptr);
    }
}

void UserLoginScn::on_btnReg_clicked()
{
    UserRegDialog dialog;
    dialog.exec();
}

void UserLoginScn::on_btnClose_clicked()
{
    this->close();
}

void UserLoginScn::setRoundWidget()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),15,15);
    setMask(bmp);
}

void UserLoginScn::mouseMoveEvent(QMouseEvent *event)
{
    if (press)
    {
        this->move(this->pos() + event->globalPos() - oldPos);
        oldPos = event->globalPos();
    }
}

void UserLoginScn::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        oldPos = event->globalPos();
        press = true;
    }
}

void UserLoginScn::on_btnMini_clicked()
{
    showMinimized();
}

void UserLoginScn::on_btnBackup_clicked()
{
    UserBackupDialog dialog;
    dialog.exec();
}
