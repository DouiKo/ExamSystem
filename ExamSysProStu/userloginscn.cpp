#include "userloginscn.h"
#include "ui_userloginscn.h"
#include"qmessagebox.h"
#include"qjsondocument.h"
#include"qjsonobject.h"
#include"qjsonvalue.h"
#include"userregdialog.h"
#include"userdata.h"
#include <QBitmap>
#include <QMouseEvent>
#include <qpainter.h>
#include"userbackupdialog.h"

extern ExTcpSocket *tcpClient;
extern QString currUserName;

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
            QString classid = jsonObject.value("class").toString();
            QString question = jsonObject.value("question").toString();
            QString result = jsonObject.value("result").toString();
            UserData user("1",password,phone,school,classid,question,result);
            currUserName = username;
            mainScn = new UserMainScn(user);    //缓式加载，提升性能
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
    UserData user("1","123","123","123","123","123","123");
    currUserName = "1";
    mainScn = new UserMainScn(user);    //缓式加载，提升性能
    mainScn->show();
    this->close();
    QString userName = ui->leUser->text();
    QString passWord = ui->lePass->text();
    if(userName != "" && passWord != ""){
        QJsonObject jsonObject;
        jsonObject.insert("role","student");
        jsonObject.insert("username",userName);
        jsonObject.insert("password",passWord);
        tcpClient->sendMsg("login",jsonObject);
    }else{
        QMessageBox::information(nullptr,"登录","用户名或密码不能为空",nullptr,nullptr);
    }
}

void UserLoginScn::on_btnReg_clicked()
{
    UserRegDialog dialog;
    dialog.exec();
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

void UserLoginScn::on_btnClose_clicked()
{
    this->close();
}

void UserLoginScn::on_btnBackup_clicked()
{
    UserBackupDialog dialog;
    dialog.exec();
}
