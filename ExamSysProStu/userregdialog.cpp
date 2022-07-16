#include "userregdialog.h"
#include "ui_userregdialog.h"
#include"qmessagebox.h"
#include"qjsonobject.h"
#include"extcpsocket.h"

extern ExTcpSocket *tcpClient;

UserRegDialog::UserRegDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserRegDialog)
{
    ui->setupUi(this);
    setWindowTitle("用户注册");
    connect(tcpClient,&ExTcpSocket::receiveData,this,&UserRegDialog::receiveMsg);
}

UserRegDialog::~UserRegDialog()
{
    delete ui;
}

void UserRegDialog::receiveMsg(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType;
    in >> msgType;
    qDebug()<<msgType;
    if(msgType == "regdone"){
        QJsonObject jsonObject;
        in >> jsonObject;
        if(jsonObject.value("error").toString() == "none"){
            QMessageBox::information(nullptr,"注册","注册成功，返回登录");
            this->close();
        }
        if(jsonObject.value("error").toString() == "failure"){
            QMessageBox::information(nullptr,"注册","注册失败",nullptr,nullptr);
        }
    }
}

void UserRegDialog::on_btnReg_clicked()
{
    QString textUserName = ui->leUser->text();
    QString textPassWord = ui->lePass->text();
    QString textPhoneNumber = ui->lePhone->text();
    QString textClassId = ui->spinBoxClassNumber->text();
    QString textStudentId = ui->spinBoxStuNumber->text();
    QString textSchoolName = ui->leSchoolName->text();
    QString textQues = ui->leQues->text();
    QString textResult = ui->leResult->text();
    if(textUserName != "" && textUserName != "" && textPhoneNumber != "" && textClassId != "" && textStudentId != "" && textSchoolName != ""){
        QJsonObject jsonObject;
        jsonObject.insert("role","student");
        jsonObject.insert("username",textUserName);
        jsonObject.insert("password",textPassWord);
        jsonObject.insert("phonenumber",textPhoneNumber);
        jsonObject.insert("classid",textClassId);
        jsonObject.insert("studentid",textStudentId);
        jsonObject.insert("schoolname",textSchoolName);
        jsonObject.insert("question",textQues);
        jsonObject.insert("result",textResult);
        tcpClient->sendMsg("reg",jsonObject);
    }else{
        QMessageBox::information(nullptr,"注册","信息填写不完整",nullptr,nullptr);
    }
}
