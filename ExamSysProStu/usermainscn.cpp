#include "usermainscn.h"
#include "ui_usermainscn.h"
#include"extcpsocket.h"
#include"qsqlquery.h"
#include"qmessagebox.h"
#include <qdatetime.h>
#include"examinationdialog.h"
#include"qsqlerror.h"

extern QString currUserName;
extern ExTcpSocket *tcpClient;
extern QSqlQuery outQuery;
extern QSqlQuery inQuery;

UserMainScn::UserMainScn(UserData user, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserMainScn)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    currClassid = user.classid;
    floatLabelAx = 960;
    floatLabelTimer = new QTimer(this);
    connect(floatLabelTimer,&QTimer::timeout,this,&UserMainScn::onFloatLabelTimeout);
    floatLabelTimer->start(30);
    searchExamTimer = new QTimer(this);
    connect(searchExamTimer,&QTimer::timeout,this,&UserMainScn::onSearchExamTimeout);
    QJsonObject jsonObject;
    jsonObject.insert("school",user.school);
    jsonObject.insert("classid",user.classid);
    tcpClient->sendMsg("searchexam",jsonObject);
    connect(tcpClient,&ExTcpSocket::receiveData,this,&UserMainScn::onReceiveMsg);
    ui->floatLabel->setText("欢迎使用，用户"+currUserName+"，"+"当前暂无考试!");
    ui->textUserWelcom->setText("欢迎使用，用户"+currUserName);
    initSelectModel();
    ui->leUser->setText(user.userName);
    ui->lePass->setText(user.passWord);
    ui->lePhone->setText(user.phone);
    ui->leSchoolName->setText(user.school);
    ui->spinBoxClassNumber->setValue(user.classid.toInt());
    ui->leQues->setText(user.question);
    ui->leResult->setText(user.result);
}

UserMainScn::~UserMainScn()
{
    delete ui;
}

void UserMainScn::onReceiveMsg(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType;
    in >> msgType;
    if(msgType == "searchexam"){
        inQuery.exec("delete from exam_record");
        examStartTimeMap.clear();
        examStopTimeMap.clear();
        QJsonArray jsonArray;
        in >> jsonArray;
        foreach (QJsonValue jsonValue, jsonArray) {
            QJsonObject jsonObject = jsonValue.toObject();
            QString examid = jsonObject.value("examid").toString();
            QString startTime = jsonObject.value("starttime").toString();
            QString stopTime = jsonObject.value("stoptime").toString();
            QString school = jsonObject.value("school").toString();
            QString classid = jsonObject.value("classid").toString();
            QString teacher = jsonObject.value("teacher").toString();
            inQuery.exec("insert into exam_record (examid,starttime,stoptime,school,class,teacher)values"
                         "('"+examid+"','"+startTime+"','"+stopTime+"','"+school+"','"+classid+"','"+teacher+"')");
        }
        searchExamTimer->start(1000);
    }
    if(msgType == "getexam"){
        in >> examArray;
        foreach (QJsonValue jsonValue, examArray) {
            QJsonObject jsonObject = jsonValue.toObject();
            if(jsonObject.contains("examName"))ui->textExamName->setText(jsonObject.value("examName").toString());
            if(jsonObject.contains("singleNumber"))ui->textExamSingle->setText(jsonObject.value("singleNumber").toString());
            if(jsonObject.contains("muchNumber"))ui->textExamMuch->setText(jsonObject.value("muchNumber").toString());
            if(jsonObject.contains("tofNumber"))ui->textExamToF->setText(jsonObject.value("tofNumber").toString());
            if(jsonObject.contains("teacher"))ui->textExamTeacher->setText(jsonObject.value("teacher").toString());
        }
        ui->floatLabel->setText("欢迎使用，用户"+currUserName+"，"+"考试即将开始，请准备!");
    }
    if(msgType == "getfreeexam"){
        in >> examArray;
        qDebug()<<examArray;
        foreach (QJsonValue jsonValue, examArray) {
            QJsonObject jsonObject = jsonValue.toObject();
            if(jsonObject.contains("examName"))ui->textFreeExamName->setText(jsonObject.value("examName").toString());
            if(jsonObject.contains("singleNumber"))ui->textFreeSingleNumber->setText(jsonObject.value("singleNumber").toString());
            if(jsonObject.contains("muchNumber"))ui->textFreeMuchNumber->setText(jsonObject.value("muchNumber").toString());
            if(jsonObject.contains("tofNumber"))ui->textFreeTofNumber->setText(jsonObject.value("tofNumber").toString());
            if(jsonObject.contains("teacher"))ui->textFreeTeacher->setText(jsonObject.value("teacher").toString());
            if(jsonObject.contains("singleScore"))ui->textFreeSingleScore->setText(jsonObject.value("singleScore").toString());
            if(jsonObject.contains("muchScore"))ui->textFreeMuchScore->setText(jsonObject.value("muchScore").toString());
            if(jsonObject.contains("tofScore"))ui->textFreeTofScore->setText(jsonObject.value("tofScore").toString());
            if(jsonObject.contains("allScore"))ui->textFreeAllScore->setText(jsonObject.value("allScore").toString());
        }
        if(!examArray.isEmpty()){
            ui->btnStartExercise->setText("开始练习");
        }else{
            QMessageBox::information(nullptr,"自主连接","未查询到该试卷，请确认编号正确!");
        }
    }
}

void UserMainScn::onSearchExamTimeout()
{
    outQuery.exec("select * from exam_record where state = '未参与'");
    examStartTimeMap.clear();
    examStopTimeMap.clear();
    while (outQuery.next()) {
        QDateTime startDateTime = QDateTime::fromString(outQuery.value(1).toString(),"yyyy-M-d h:m:s");
        QDateTime stopDateTime = QDateTime::fromString(outQuery.value(2).toString(),"yyyy-M-d h:m:s");
        if(startDateTime <= QDateTime::currentDateTime() && stopDateTime >= QDateTime::currentDateTime()){
            examStartTimeMap[outQuery.value(0).toString()] = startDateTime;
            examStopTimeMap[outQuery.value(0).toString()] = stopDateTime;
        }
    }
    if(!examStartTimeMap.isEmpty()){
        if(examStartTimeMap.values().first() <= QDateTime::currentDateTime()){
            QString startTime = examStartTimeMap.values().first().toString("yyyy-M-d h:mm:ss");
            QString stopTime = examStopTimeMap.values().first().toString("yyyy-M-d h:mm:ss");
            QString examid = examStartTimeMap.keys().first();
            qDebug()<<startTime;
            outQuery.exec("select * from exam_record where examid = '"+examid+"' and starttime = '"+startTime+"' and stoptime = '"+stopTime+"'");
            qDebug()<<outQuery.lastError().text();
            if(outQuery.next()){
                ui->textExamid->setText(outQuery.value(0).toString());
                ui->textStartTime->setText(outQuery.value(1).toString());
                ui->textStopTime->setText(outQuery.value(2).toString());
                ui->textExamSchool->setText(outQuery.value(3).toString());
                ui->textExamClass->setText(outQuery.value(4).toString());
            }
            QJsonObject jsonObject;
            jsonObject.insert("examid",examStartTimeMap.keys().first());
            if(currHasExam != 1){
                tcpClient->sendMsg("getexam",jsonObject);
                currHasExam = 1;
            }
            searchExamTimer->stop();
        }else{
            currHasExam = 0;
        }
    }
}

void UserMainScn::onFloatLabelTimeout()
{
    floatLabelAx--;
    ui->floatLabel->setGeometry(floatLabelAx,ui->floatLabel->y(),ui->floatLabel->width(),ui->floatLabel->height());
    if(floatLabelAx < -250){
        floatLabelAx = 980;
    }
}


void UserMainScn::on_btnStartExam_clicked()
{
    if(currHasExam == 1){
        inQuery.exec("update exam_record set state = '已参与' where examid = '"+examStartTimeMap.keys().first()+"'");
        ExaminationDialog dialog(examStartTimeMap.values().first(),examStopTimeMap.values().first(),currUserName,currClassid,examArray,examStartTimeMap.keys().first());
        this->hide();
        dialog.exec();
        this->show();
        ui->floatLabel->setText("欢迎使用，用户"+currUserName+"，"+"当前暂无考试!");
        ui->textExamid->setText("当前暂无考试");
        ui->textExamName->setText("当前暂无考试");
        ui->textStartTime->setText("当前暂无考试");
        ui->textStopTime->setText("当前暂无考试");
        ui->textExamTeacher->setText("当前暂无考试");
        ui->textExamSchool->setText("当前暂无考试");
        ui->textExamClass->setText("当前暂无考试");
        ui->textExamSingle->setText("当前暂无考试");
        ui->textExamMuch->setText("当前暂无考试");
        ui->textExamToF->setText("当前暂无考试");
        examStartTimeMap.remove(examStartTimeMap.keys().first());
        examStopTimeMap.remove(examStopTimeMap.keys().first());
        currHasExam = 0;
    }else{
        QMessageBox::information(nullptr,"开始考试","考试暂未开始，请耐心等待!");
    }
}

void UserMainScn::on_btnClose_clicked()
{
    this->close();
}

void UserMainScn::on_btnMini_clicked()
{
    showMinimized();
}

void UserMainScn::on_pushButton_5_clicked()
{
    showMaximized();
}

void UserMainScn::on_btnStartExercise_clicked()
{
    if(ui->btnStartExercise->text() == "查询试卷"){
        QJsonObject jsonObject;
        jsonObject.insert("examid",ui->leFreeExamid->text());
        tcpClient->sendMsg("getfreeexam",jsonObject);
    }
    if(ui->btnStartExercise->text()=="开始练习"){
        if(ui->cbShowResult->isChecked()){
            ExaminationDialog dialog(QDateTime(),QDateTime(),currUserName,currClassid,examArray,ui->leFreeExamid->text(),2);
            this->hide();
            dialog.exec();
            this->show();
            ui->btnStartExercise->setText("查询试卷");
            ui->leFreeExamid->clear();
        }else{
            ExaminationDialog dialog(QDateTime(),QDateTime(),currUserName,currClassid,examArray,ui->leFreeExamid->text(),1);
            this->hide();
            dialog.exec();
            this->show();
            ui->leFreeExamid->clear();
            ui->btnStartExercise->setText("查询试卷");
        }
    }
}

void UserMainScn::on_btnSelectModel_clicked()
{
    QString text = ui->leSelect->text();
    selectModel->setFilter(QObject::tr("(username = '%1') and (examid like '%%2%') or  (examname like '%%3%')").arg(currUserName).arg(text).arg(text));
    selectModel->select();
}

void UserMainScn::initSelectModel()
{
    selectModel = new QSqlTableModel(this);
    selectModel->setTable("examlog");
    selectModel->setFilter(QObject::tr("username = '%1'").arg(currUserName));
    selectModel->select();
    selectModel->setHeaderData(0,Qt::Orientation::Horizontal,"试卷编号");
    selectModel->setHeaderData(1,Qt::Orientation::Horizontal,"试卷名称");
    selectModel->setHeaderData(2,Qt::Orientation::Horizontal,"单选数量");
    selectModel->setHeaderData(3,Qt::Orientation::Horizontal,"多选数量");
    selectModel->setHeaderData(4,Qt::Orientation::Horizontal,"判断数量");
    selectModel->setHeaderData(5,Qt::Orientation::Horizontal,"合计数量");
    selectModel->setHeaderData(6,Qt::Orientation::Horizontal,"单选得分");
    selectModel->setHeaderData(7,Qt::Orientation::Horizontal,"多选得分");
    selectModel->setHeaderData(8,Qt::Orientation::Horizontal,"判断得分");
    selectModel->setHeaderData(9,Qt::Orientation::Horizontal,"合计得分");
    selectModel->setHeaderData(10,Qt::Orientation::Horizontal,"发起教师");
    selectModel->setHeaderData(11,Qt::Orientation::Horizontal,"考试用时");
    selectModel->setHeaderData(12,Qt::Orientation::Horizontal,"参与考生");
    ui->selecttableView->setModel(selectModel);
    ui->selecttableView->verticalHeader()->hide();
    ui->selecttableView->setColumnWidth(0,80);
    ui->selecttableView->setColumnWidth(1,80);
    ui->selecttableView->setColumnWidth(2,80);
    ui->selecttableView->setColumnWidth(3,80);
    ui->selecttableView->setColumnWidth(4,80);
    ui->selecttableView->setColumnWidth(5,80);
    ui->selecttableView->setColumnWidth(6,80);
    ui->selecttableView->setColumnWidth(7,80);
    ui->selecttableView->setColumnWidth(8,80);
    ui->selecttableView->setColumnWidth(9,80);
    ui->selecttableView->setColumnWidth(10,80);
    ui->selecttableView->setColumnWidth(11,80);
    ui->selecttableView->setColumnWidth(12,80);
    ui->selecttableView->setAlternatingRowColors(true);
}

#include"chartdialog.h"

void UserMainScn::on_btnShowExamInfo_clicked()
{
    double singleScore = selectModel->index(ui->selecttableView->currentIndex().row(),6).data().toString().toDouble();
    double muchScore = selectModel->index(ui->selecttableView->currentIndex().row(),7).data().toString().toDouble();
    double tofScore = selectModel->index(ui->selecttableView->currentIndex().row(),8).data().toString().toDouble();
    double allScore = selectModel->index(ui->selecttableView->currentIndex().row(),9).data().toString().toDouble();
    double rightSingle = 0.0,rightMuch = 0.0,rightTof = 0.0;
    if(allScore > 0){
        rightSingle = singleScore / allScore;
        rightMuch = muchScore / allScore;
        rightTof = tofScore / allScore;
    }
    ChartDialog chart(rightSingle,rightMuch,rightTof);
    chart.exec();
}

void UserMainScn::on_btnErrorReset_clicked()
{
    QString examid = selectModel->index(ui->selecttableView->currentIndex().row(),0).data().toString();
    QString time = selectModel->index(ui->selecttableView->currentIndex().row(),11).data().toString();
    time = time.remove("/分钟");
    QFile file(examid+"#"+time+".json");
    file.open(QIODevice::ReadOnly);
    QByteArray data=file.readAll();
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(data);
    QJsonArray jsonArray = doc.array();
    ExaminationDialog dialog(QDateTime(),QDateTime(),currUserName,currClassid,jsonArray,ui->leFreeExamid->text(),2);
    this->hide();
    dialog.exec();
    this->show();
}

void UserMainScn::on_btnCloseSys_clicked()
{
    this->close();
}

void UserMainScn::on_btnSetUserData_clicked()
{
    QString username = ui->leUser->text();
    QString password = ui->lePass->text();
    QString phone = ui->lePhone->text();
    QString school = ui->leSchoolName->text();
    QString classid = ui->spinBoxClassNumber->text();
    QString question = ui->leQues->text();
    QString result = ui->leResult->text();
    QJsonObject jsonObject;
    jsonObject.insert("username",username);
    jsonObject.insert("password",password);
    jsonObject.insert("phone",phone);
    jsonObject.insert("school",school);
    jsonObject.insert("studentid",school);
    jsonObject.insert("class",classid);
    jsonObject.insert("question",question);
    jsonObject.insert("result",result);
    jsonObject.insert("role",QString("student"));
    if(tcpClient->sendMsg("resetuser",jsonObject)){
        QMessageBox::information(nullptr,"修改个人资料","修改成功，信息已提交至服务器");
    }else{
        QMessageBox::information(nullptr,"修改个人资料","修改失败");
    }
}
