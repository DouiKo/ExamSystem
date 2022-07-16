#include "examinationdialog.h"
#include "ui_examinationdialog.h"
#include"qmessagebox.h"
#include"qdebug.h"
#include"QCloseEvent"
#include"quesitem.h"
#include"qjsonarray.h"
#include"qjsonobject.h"
#include"qjsondocument.h"
#include"quesbutton.h"
#include"extcpsocket.h"
#include"qsqlquery.h"
#include"qsqlerror.h"

extern ExTcpSocket *tcpClient;
extern QString currUserName;
extern QSqlQuery outQuery;
extern QSqlQuery inQuery;

ExaminationDialog::ExaminationDialog(QDateTime startTime, QDateTime stopTime, QString username, QString classid, QJsonArray jsonArray, QString examid, int type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExaminationDialog)
{
    ui->setupUi(this);
    quesArray = jsonArray;
    this->examid = examid;
    startDateTime = startTime;
    stopDateTime = stopTime;
    examType = type;
    ui->textTime->setText(startDateTime.toString("yyyy-MM-dd hh:mm:ss") + " - "+stopDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    ui->plainTextEditUser->setPlainText(username);
    ui->plainTextEditClass->setPlainText(classid);
    QJsonObject infoJson = jsonArray.last().toObject();
    QString examName = infoJson.value("examName").toString();
    setWindowTitle(examName);
    int quesCount = 0;
    int singleCount = 0;
    int muchCount = 0;
    int tofCount = 0;
    foreach (QJsonValue jsonValue, jsonArray) {
        QJsonObject jsonObject = jsonValue.toObject();
        QString title = jsonObject.value("title").toString();
        QString c1 = jsonObject.value("c1").toString();
        QString c2 = jsonObject.value("c2").toString();
        QString c3 = jsonObject.value("c3").toString();
        QString c4 = jsonObject.value("c4").toString();
        QString c5 = jsonObject.value("c5").toString();
        QString c6 = jsonObject.value("c6").toString();
        QString result = jsonObject.value("result").toString();
        QString type = jsonObject.value("type").toString();
        QString score = jsonObject.value("score").toString();
        qDebug()<<jsonObject;
        if(examType == 2){
            title = title + "\n\n\n解析:"+result;
        }
        if(type == "单选"){
            QuesItem *item = new QuesItem(0,title,quesCount);
            ui->stackedWidget->addWidget(item);
            if(c1 != "")item->addNewChoice(c1);
            if(c2 != "")item->addNewChoice(c2);
            if(c3 != "")item->addNewChoice(c3);
            if(c4 != "")item->addNewChoice(c4);
            if(c5 != "")item->addNewChoice(c5);
            if(c6 != "")item->addNewChoice(c6);
            item->setTrueResult(result);
            item->setScore(score.toInt());
            QuesButton *button = new QuesButton(this);
            button->setMinimumSize(30,30);
            button->setMaximumSize(30,30);
            button->setProperty("page",quesCount);
            button->setText(QString::number(quesCount+1));
            connect(button,&QuesButton::clicked,this,&ExaminationDialog::onQuesButtonClicked);
            quesMap[item] = button;
            ui->single_gridLayout->addWidget(button,singleCount/4,singleCount%4);
            connect(item,&QuesItem::quesStateChanged,this,&ExaminationDialog::onQuesStateChanged);
            quesList.append(item);
            singleCount++;
        }
        if(type == "多选"){
            QuesItem *item = new QuesItem(1,title,quesCount);
            ui->stackedWidget->addWidget(item);
            if(c1 != "")item->addNewChoice(c1);
            if(c2 != "")item->addNewChoice(c2);
            if(c3 != "")item->addNewChoice(c3);
            if(c4 != "")item->addNewChoice(c4);
            if(c5 != "")item->addNewChoice(c5);
            if(c6 != "")item->addNewChoice(c6);
            item->setTrueResult(result);
            item->setScore(score.toInt());
            QuesButton *button = new QuesButton(this);
            button->setMinimumSize(30,30);
            button->setMaximumSize(30,30);
            button->setProperty("page",quesCount);
            button->setText(QString::number(quesCount+1));
            connect(button,&QuesButton::clicked,this,&ExaminationDialog::onQuesButtonClicked);
            quesMap[item] = button;
            ui->much_gridLayout->addWidget(button,muchCount/4,muchCount%4);
            connect(item,&QuesItem::quesStateChanged,this,&ExaminationDialog::onQuesStateChanged);
            quesList.append(item);
            muchCount++;
        }
        if(type == "判断"){
            QuesItem *item = new QuesItem(2,title,quesCount);
            ui->stackedWidget->addWidget(item);
            if(c1 != "")item->addNewChoice(c1);
            if(c2 != "")item->addNewChoice(c2);
            if(c3 != "")item->addNewChoice(c3);
            if(c4 != "")item->addNewChoice(c4);
            if(c5 != "")item->addNewChoice(c5);
            if(c6 != "")item->addNewChoice(c6);
            item->setTrueResult(result);
            item->setScore(score.toInt());
            QuesButton *button = new QuesButton(this);
            button->setMinimumSize(30,30);
            button->setMaximumSize(30,30);
            button->setProperty("page",quesCount);
            button->setText(QString::number(quesCount+1));
            connect(button,&QuesButton::clicked,this,&ExaminationDialog::onQuesButtonClicked);
            quesMap[item] = button;
            ui->tof_gridLayout->addWidget(button,tofCount/4,tofCount%4);
            connect(item,&QuesItem::quesStateChanged,this,&ExaminationDialog::onQuesStateChanged);
            quesList.append(item);
            tofCount++;
        }
        quesCount++;
    }
    flag = startTimer(1000);
}

ExaminationDialog::~ExaminationDialog()
{
    delete ui;
}

void ExaminationDialog::onQuesButtonClicked()
{
    QuesButton *button = qobject_cast<QuesButton *>(sender());
    ui->stackedWidget->setCurrentIndex(button->property("page").toInt());
}

void ExaminationDialog::onQuesStateChanged()
{
    QuesItem *item = qobject_cast<QuesItem *>(sender());
    if(item->getSelectItemId() != ""){
        quesMap[item]->setDoneState(true);
    }else{
        quesMap[item]->setDoneState(false);
    }
}

void ExaminationDialog::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == flag){
        if(examType == 0){
            if(QDateTime::currentDateTime() >= stopDateTime){
                this->close();
                killTimer(flag);
            }
        }
        int startSec = startDateTime.secsTo(QDateTime::currentDateTime());
        startSec = startSec / 60;
        ui->textUsedTime->setText(QString::number(startSec)+"/分钟");

        int stopSec = QDateTime::currentDateTime().secsTo(stopDateTime);
        stopSec = stopSec / 60;
        ui->textStopTime->setText(QString::number(stopSec)+"/分钟");
    }
}

void ExaminationDialog::closeEvent(QCloseEvent *e)
{
    int ret = QMessageBox::information(nullptr,"结束考试","确认结束考试吗？考试结果将自动提交",QMessageBox::Ok,QMessageBox::No);
    if(ret == QMessageBox::Ok){
        if(examType == 0){
            billExam();
        }
        e->accept();
    }else{
        e->ignore();
    }
}

void ExaminationDialog::createErrorJson(QuesItem *item)
{
    QJsonObject jsonObject;
    if(item->getQuesType() == 0){
        jsonObject.insert("type",QString("单选"));
    }
    if(item->getQuesType() == 1){
        jsonObject.insert("type",QString("多选"));
    }
    if(item->getQuesType() == 2){
        jsonObject.insert("type",QString("判断"));
    }
    jsonObject.insert("title",item->getTitle());
    jsonObject.insert("c1",item->getChoice(0));
    jsonObject.insert("c2",item->getChoice(1));
    jsonObject.insert("c3",item->getChoice(2));
    jsonObject.insert("c4",item->getChoice(3));
    jsonObject.insert("c5",item->getChoice(4));
    jsonObject.insert("c6",item->getChoice(5));
    jsonObject.insert("score",item->getScore());
    jsonObject.insert("result",item->getTrueResult());
    errorArray.append(jsonObject);
}

void ExaminationDialog::on_btnClose_clicked()
{
    this->close();
}

void ExaminationDialog::billExam()
{
    int singleScore = 0;
    int muchScore = 0;
    int tofScore = 0;
    int allScore = 0;
    foreach (QuesItem *item, quesList) {
        if(item->getQuesType() == 0){
            if(item->getTrueResult() == item->getSelectItemId()){
                singleScore += item->getScore();
            }else{
                createErrorJson(item);
            }
        }
        if(item->getQuesType() == 1){
            QString selectResult = item->getSelectItemId();
            selectResult = selectResult.remove(selectResult.size()-1,1);
            QStringList selectResultList = selectResult.split(QRegExp(" "));
            QString trueResult = item->getTrueResult();
            bool isError = false;
            for (int i = 0; i < selectResultList.size(); ++i) {
                if(trueResult.contains(selectResultList.value(i))){
                    trueResult.remove(selectResultList.value(i)+" ");
                }else{
                    isError = true;
                    break;
                }
            }
            if(!isError&&trueResult==""){
                muchScore += item->getScore();
            }else{
                createErrorJson(item);
            }
        }
        if(item->getQuesType() == 2){
            if(item->getTrueResult() == item->getSelectItemId()){
                tofScore += item->getScore();
            }else{
                createErrorJson(item);
            }
        }
    }
    allScore = singleScore + muchScore + tofScore;
    QJsonObject infoJson = quesArray.last().toObject();
    QString examName = infoJson.value("examName").toString();
    QString singleNumber = infoJson.value("singleNumber").toString();
    QString muchNumber = infoJson.value("muchNumber").toString();
    QString tofNumber = infoJson.value("tofNumber").toString();
    QString allNumber = infoJson.value("allNumber").toString();
    QString teacher = infoJson.value("teacher").toString();

    QJsonObject jsonObject;
    jsonObject.insert("examid",examid);
    jsonObject.insert("examName",examName);
    jsonObject.insert("singleNumber",singleNumber);
    jsonObject.insert("muchNumber",muchNumber);
    jsonObject.insert("tofNumber",tofNumber);
    jsonObject.insert("allNumber",allNumber);
    jsonObject.insert("singleScore",QString::number(singleScore));
    jsonObject.insert("muchScore",QString::number(muchScore));
    jsonObject.insert("tofScore",QString::number(tofScore));
    jsonObject.insert("allScore",QString::number(allScore));
    jsonObject.insert("teacher",teacher);
    jsonObject.insert("usedtime",ui->textUsedTime->text());
    jsonObject.insert("username",currUserName);

    tcpClient->sendMsg("billexam",jsonObject);
    inQuery.exec("insert into examlog (examid,examname,singlenumber,muchnumber,tofnumber,allnumber,singlescore,muchscore,tofscore,allscore,teacher,usedtime,username)values"
                 "('"+examid+"','"+examName+"','"+singleNumber+"','"+muchNumber+"','"+tofNumber+"','"+allNumber+"','"+QString::number(singleScore)+"','"+QString::number(muchScore)+"','"+QString::number(tofScore)+"','"+QString::number(allScore)+"','"+teacher+"','"+ui->textUsedTime->text()+"','"+currUserName+"')");
    qDebug()<<inQuery.lastError().text();

    QJsonDocument jsonDocumen(errorArray);
    QByteArray examJsonData = jsonDocumen.toJson(QJsonDocument::Indented);
    QString time = ui->textUsedTime->text();
    time = time.remove("/分钟");
    QFile file(examid+"#"+time+".json");
    file.open(QIODevice::WriteOnly);
    file.write(examJsonData);
    file.close();
}
