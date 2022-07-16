#include "userbackupdialog.h"
#include "ui_userbackupdialog.h"
#include"QMessageBox"
#include"extcpsocket.h"

#include <QJsonObject>

extern ExTcpSocket *tcpClient;

UserBackupDialog::UserBackupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserBackupDialog)
{
    ui->setupUi(this);
    setWindowTitle("找回密码");
    connect(tcpClient,&ExTcpSocket::receiveData,this,&UserBackupDialog::receiveMsg);
}

UserBackupDialog::~UserBackupDialog()
{
    delete ui;
}

void UserBackupDialog::receiveMsg(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType;
    in >> msgType;
    if(msgType == "userbackup"){
        QJsonObject jsonObject;
        in >> jsonObject;
        if(jsonObject.value("error").toString() == "noerror"){
            QString backUser = jsonObject.value("username").toString();
            QString backPass = jsonObject.value("password").toString();
            QMessageBox::information(nullptr,"找回密码","找回成功，请牢记您的账号信息!\n您的用户名是:"+backUser+" \n"+"您的密码是:"+backPass);
        }
        if(jsonObject.value("error").toString() == "error"){
            QMessageBox::information(nullptr,"找回密码","找回密码失败，信息不足!");
        }
    }
}

void UserBackupDialog::on_btnReturnPassword_clicked()
{
    QString username = ui->leUser->text();
    QString phone = ui->lePhone->text();
    QString school = ui->leSchoolName->text();
    QString classId = ui->leClassNumber->text();
    QString result = ui->leResult->text();
    QString role = "student";

    QJsonObject jsonObject;
    jsonObject.insert("username",username);
    jsonObject.insert("phone",phone);
    jsonObject.insert("school",school);
    jsonObject.insert("class",classId);
    jsonObject.insert("result",result);
    jsonObject.insert("role",role);
    tcpClient->sendMsg("userbackup",jsonObject);
}
