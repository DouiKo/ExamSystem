#include "usermainscn.h"
#include "ui_usermainscn.h"
#include"extcpsocket.h"

#include <QJsonObject>
#include <QThread>
#include <qdatetime.h>
#include <qjsonarray.h>
#include"xlsx/xlsxdocument.h"
#include"qfiledialog.h"
#include"qprocess.h"
#include"qsqlquery.h"
#include"qscrollbar.h"
#include"qpoint.h"
#include"qmessagebox.h"
#include"qsqlerror.h"

extern ExTcpSocket *tcpClient;
extern QSqlQuery outQuery;
extern QSqlQuery inQuery;
extern QString currUserName;

UserMainScn::UserMainScn(UserData user, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserMainScn)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    currUserName = user.userName;
    connect(tcpClient,&ExTcpSocket::receiveData,this,&UserMainScn::receiveMsg);
    tcpClient->sendMsg("getquestion",QJsonObject());
    initSelectModel();
    initSelectExamListModel();
    ui->textUserWelcom->setText("欢迎使用，用户"+currUserName);
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

void UserMainScn::receiveMsg(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType;
    in >> msgType;
    if(msgType == "getquestion"){
        QJsonArray jsonArray;
        in >> jsonArray;
        inQuery.exec("delete from queslist");
        inQuery.exec("update sqlite_sequence set seq=0 where name = 'queslist'");
        foreach (QJsonValue jsonValue, jsonArray) {
            QJsonObject jsonObject = jsonValue.toObject();
            QString title = jsonObject.value("title").toString();
            QString type = jsonObject.value("type").toString();
            QString c1 = jsonObject.value("c1").toString();
            QString c2 = jsonObject.value("c2").toString();
            QString c3 = jsonObject.value("c3").toString();
            QString c4 = jsonObject.value("c4").toString();
            QString c5 = jsonObject.value("c5").toString();
            QString c6 = jsonObject.value("c6").toString();
            QString result = jsonObject.value("result").toString();
            QString level = jsonObject.value("level").toString();
            QString score = jsonObject.value("score").toString();
            QString teacher = jsonObject.value("teacher").toString();
            inQuery.exec("insert into queslist (ex_title,ex_type,ex_1,ex_2,ex_3,ex_4,ex_5,ex_6,ex_result,ex_level,ex_score,ex_teacher)"
                         "values('"+title+"','"+type+"','"+c1+"','"+c2+"','"+c3+"','"+c4+"','"+c5+"','"+c6+"','"+result+"','"+level+"','"+score+"','"+teacher+"')");
        }
        tcpClient->sendMsg("getexamlist",QJsonObject());
    }
    if(msgType == "getexamlist"){
        QJsonArray jsonArray;
        in >> jsonArray;
        inQuery.exec("delete from examlist");
        inQuery.exec("update sqlite_sequence set seq=0 where name = 'examlist'");
        foreach (QJsonValue jsonValue, jsonArray) {
            QJsonObject jsonObject = jsonValue.toObject();
            QString examid = jsonObject.value("examid").toString();
            QString examname = jsonObject.value("examname").toString();
            QString singlenumber = jsonObject.value("singlenumber").toString();
            QString muchnumber = jsonObject.value("muchnumber").toString();
            QString tofnumber = jsonObject.value("tofnumber").toString();
            QString singlescore = jsonObject.value("singlescore").toString();
            QString muchscore = jsonObject.value("muchscore").toString();
            QString tofscore = jsonObject.value("tofscore").toString();
            QString allnumber = jsonObject.value("allnumber").toString();
            QString allscore = jsonObject.value("allscore").toString();
            QString teacher = jsonObject.value("teacher").toString();
            inQuery.exec("insert into examlist (examid,examname,singlenumber,muchnumber,tofnumber,singlescore,muchscore,tofscore,allnumber,allscore,teacher)"
                         "values('"+examid+"','"+examname+"','"+singlenumber+"','"+muchnumber+"','"+tofnumber+"','"+singlescore+"','"+muchscore+"','"+tofscore+"','"+allnumber+"','"+allscore+"','"+teacher+"')");
        }
        QJsonObject sendJson;
        sendJson.insert("teacher",currUserName);
        tcpClient->sendMsg("getexamlog",sendJson);
    }
    if(msgType == "getexamlog"){
        QJsonArray jsonArray;
        in >> jsonArray;
        inQuery.exec("delete from examlog");
        foreach (QJsonValue jsonValue, jsonArray) {
            QJsonObject jsonObject = jsonValue.toObject();
            QString examid = jsonObject.value("examid").toString();
            QString examname = jsonObject.value("examname").toString();
            QString singlenumber = jsonObject.value("singlenumber").toString();
            QString muchnumber = jsonObject.value("muchnumber").toString();
            QString tofnumber = jsonObject.value("tofnumber").toString();
            QString singlescore = jsonObject.value("singlescore").toString();
            QString muchscore = jsonObject.value("muchscore").toString();
            QString tofscore = jsonObject.value("tofscore").toString();
            QString allnumber = jsonObject.value("allnumber").toString();
            QString allscore = jsonObject.value("allscore").toString();
            QString teacher = jsonObject.value("teacher").toString();
            QString usedtime = jsonObject.value("usedtime").toString();
            QString username = jsonObject.value("username").toString();
            inQuery.exec("insert into examlog (examid,examname,singlenumber,muchnumber,tofnumber,allnumber,singlescore,muchscore,tofscore,allscore,teacher,usedtime,username)"
                         "values('"+examid+"','"+examname+"','"+singlenumber+"','"+muchnumber+"','"+tofnumber+"','"+allnumber+"','"+singlescore+"','"+muchscore+"','"+tofscore+"','"+allscore+"','"+teacher+"','"+usedtime+"','"+username+"')");
        }
        initExamLogModel();
    }
    if(msgType == "getexam"){
        QJsonArray jsonArray;
        in >> jsonArray;
        ShowExamDialog show(jsonArray);
        show.exec();
    }
}

void UserMainScn::on_btnClose_clicked()
{
    this->close();
}

void UserMainScn::on_btnMini_clicked()
{
    this->showMinimized();
}

void UserMainScn::on_btnAddSingle_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void UserMainScn::on_btnAddMuch_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void UserMainScn::on_btnAddTof_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void UserMainScn::on_btnTofReturn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserMainScn::on_btnSingleReturn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserMainScn::on_btnMuchReturn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserMainScn::on_btnAddQuesSingle_clicked()
{
    QString title = ui->leSingleTitle->text();
    QString type = "单选";
    QString c1 = ui->leSingleA->text();
    QString c2 = ui->leSingleB->text();
    QString c3 = ui->leSingleC->text();
    QString c4 = ui->leSingleD->text();
    QString c5 = ui->leSingleE->text();
    QString c6 = ui->leSingleF->text();
    QString level = ui->sbSingleLevel->text();
    QString score = ui->sbSingleScore->text();
    QString result;
    if(ui->rbSingleA->isChecked())result = c1;
    if(ui->rbSingleB->isChecked())result = c2;
    if(ui->rbSingleC->isChecked())result = c3;
    if(ui->rbSingleD->isChecked())result = c4;
    if(ui->rbSingleE->isChecked())result = c5;
    if(ui->rbSingleF->isChecked())result = c6;
    if(tcpClient->sendMsg("addquestion",addChoiceQuesJson(title,type,c1,c2,c3,c4,c5,c6,result,level,score))){
        ui->leSingleTitle->clear();
        ui->leSingleA->clear();
        ui->leSingleB->clear();
        ui->leSingleC->clear();
        ui->leSingleD->clear();
        ui->leSingleE->clear();
        ui->leSingleF->clear();
        ui->sbSingleLevel->clear();
        ui->sbSingleScore->clear();
    }
}

void UserMainScn::on_btnAddQuesMuch_clicked()
{
    QString title = ui->leMuchTitle->text();
    QString type = "多选";
    QString c1 = ui->leMuchA->text();
    QString c2 = ui->leMuchB->text();
    QString c3 = ui->leMuchC->text();
    QString c4 = ui->leMuchD->text();
    QString c5 = ui->leMuchE->text();
    QString c6 = ui->leMuchF->text();
    QString level = ui->sbMuchLevel->text();
    QString score = ui->sbMuchScore->text();
    QString result;
    if(ui->cbMuchA->isChecked())result.append(c1);
    if(ui->cbMuchB->isChecked())result.append(c2);
    if(ui->cbMuchC->isChecked())result.append(c3);
    if(ui->cbMuchD->isChecked())result.append(c4);
    if(ui->cbMuchE->isChecked())result.append(c5);
    if(ui->cbMuchF->isChecked())result.append(c6);
    if(tcpClient->sendMsg("addquestion",addChoiceQuesJson(title,type,c1,c2,c3,c4,c5,c6,result,level,score))){
        ui->leMuchTitle->clear();
        ui->leMuchA->clear();
        ui->leMuchB->clear();
        ui->leMuchC->clear();
        ui->leMuchD->clear();
        ui->leMuchE->clear();
        ui->leMuchF->clear();
        ui->sbMuchLevel->clear();
        ui->sbMuchScore->clear();
    }
}

void UserMainScn::on_btnAddQuesToF_clicked()
{
    QString title = ui->leToFTitle->text();
    QString type = "判断";
    QString c1 = ui->leTof1->text();
    QString c2 = ui->leTof2->text();
    QString level = ui->sbToFLevel->text();
    QString score = ui->sbToFScore->text();
    QString result;
    if(ui->rbTof1->isChecked())result = c1;
    if(ui->rbTof2->isChecked())result = c2;
    if(tcpClient->sendMsg("addquestion",addChoiceQuesJson(title,type,c1,c2,result,level,score))){
        ui->leToFTitle->clear();
        ui->sbToFLevel->clear();
        ui->sbToFScore->clear();
    }
}

void UserMainScn::on_btnExcelImport_clicked()
{
    QString saveFileName = QFileDialog::getOpenFileName(this,nullptr,nullptr,"Excel工作表(*.xlsx)");
    if(saveFileName != ""){
        QXlsx::Document xlsx(saveFileName);
        int rowCount = 2;
        while (xlsx.read(rowCount,1).toString() != "") {
            QString title = xlsx.read(rowCount,1).toString();
            QString type = xlsx.read(rowCount,2).toString();
            QString c1 = xlsx.read(rowCount,3).toString();
            QString c2 = xlsx.read(rowCount,4).toString();
            QString c3 = xlsx.read(rowCount,5).toString();
            QString c4 = xlsx.read(rowCount,6).toString();
            QString c5 = xlsx.read(rowCount,7).toString();
            QString c6 = xlsx.read(rowCount,8).toString();
            QString result = xlsx.read(rowCount,9).toString();
            QString level = xlsx.read(rowCount,10).toString();
            QString score = xlsx.read(rowCount,11).toString();
            QThread::msleep(100);
            QCoreApplication::processEvents(QEventLoop::AllEvents);
            qDebug()<<tcpClient->sendMsg("addquestion",addChoiceQuesJson(title,type,c1,c2,c3,c4,c5,c6,result,level,score));
            rowCount++;
        }
    }
}

QJsonObject UserMainScn::addChoiceQuesJson(QString title, QString type, QString c1, QString c2, QString c3, QString c4, QString c5, QString c6, QString result, QString level, QString score)
{
    QJsonObject jsonObject;
    if(type == "单选" || type == "多选"){
        jsonObject.insert("title",title);
        jsonObject.insert("type",type);
        jsonObject.insert("c1",c1);
        jsonObject.insert("c2",c2);
        jsonObject.insert("c3",c3);
        jsonObject.insert("c4",c4);
        jsonObject.insert("c5",c5);
        jsonObject.insert("c6",c6);
        jsonObject.insert("level",level);
        jsonObject.insert("score",score);
        jsonObject.insert("result",result);
        jsonObject.insert("teacher",currUserName);
    }
    if(type == "判断"){
        QJsonObject jsonObject;
        jsonObject.insert("title",title);
        jsonObject.insert("type",type);
        jsonObject.insert("c1",c1);
        jsonObject.insert("c2",c2);
        jsonObject.insert("level",level);
        jsonObject.insert("score",score);
        jsonObject.insert("result",result);
        jsonObject.insert("teacher",currUserName);
        return jsonObject;
    }
    return jsonObject;
}

void UserMainScn::initRandQuesArray()
{
    int hasChoice = 1;
    for (int i = 0; i < quesJsonArray.count()-1; ++i) {
        QJsonObject jsonObject = quesJsonArray[i].toObject();
        for (int j = 2; j < 8; ++j) {
            if(!jsonObject.toVariantMap().values().value(j).toString().isEmpty()){
                hasChoice++;
            }
        }
        while (hasChoice > 1) {
            qsrand(time(nullptr));
            int randCount1 = qrand()%hasChoice;
            while (randCount1 < 1) {
                randCount1 = qrand()%hasChoice;
            }
            QString c1 = jsonObject.value("c1").toString();
            QString c2 = jsonObject.value("c"+QString::number(randCount1)).toString();
            jsonObject["c1"] = c2;
            jsonObject["c"+QString::number(randCount1)] = c1;
            hasChoice--;
        }
        quesJsonArray.replace(i,jsonObject);
    }
}

void UserMainScn::on_btnShowQues_clicked()
{
    tcpClient->sendMsg("getquestion",QJsonObject());
    ui->stackedWidget->setCurrentIndex(4);
}

void UserMainScn::on_btnSelectModel_clicked()
{
    QString text = ui->leSelect->text();
    selectModel->setFilter(QObject::tr("ex_title like '%%1%'").arg(text));
    selectModel->select();
}

void UserMainScn::initSelectModel()
{
    selectModel = new QSqlTableModel(this);
    selectModel->setTable("queslist");
    selectModel->select();
    selectModel->setHeaderData(0,Qt::Orientation::Horizontal,"编号");
    selectModel->setHeaderData(1,Qt::Orientation::Horizontal,"题目");
    selectModel->setHeaderData(2,Qt::Orientation::Horizontal,"题型");
    selectModel->setHeaderData(3,Qt::Orientation::Horizontal,"选项1");
    selectModel->setHeaderData(4,Qt::Orientation::Horizontal,"选项2");
    selectModel->setHeaderData(5,Qt::Orientation::Horizontal,"选项3");
    selectModel->setHeaderData(6,Qt::Orientation::Horizontal,"选项4");
    selectModel->setHeaderData(7,Qt::Orientation::Horizontal,"选项5");
    selectModel->setHeaderData(8,Qt::Orientation::Horizontal,"选项6");
    selectModel->setHeaderData(9,Qt::Orientation::Horizontal,"答案");
    selectModel->setHeaderData(10,Qt::Orientation::Horizontal,"难度");
    selectModel->setHeaderData(11,Qt::Orientation::Horizontal,"分数");
    selectModel->setHeaderData(12,Qt::Orientation::Horizontal,"教师");
    ui->selecttableView->setModel(selectModel);
    ui->selecttableView->verticalHeader()->hide();
    ui->selecttableView->setColumnWidth(0,30);
    ui->selecttableView->setColumnWidth(1,200);
    ui->selecttableView->setColumnWidth(2,50);
    ui->selecttableView->setColumnWidth(3,80);
    ui->selecttableView->setColumnWidth(4,80);
    ui->selecttableView->setColumnWidth(5,80);
    ui->selecttableView->setColumnWidth(6,80);
    ui->selecttableView->setColumnWidth(7,80);
    ui->selecttableView->setColumnWidth(8,80);
    ui->selecttableView->setColumnWidth(9,80);
    ui->selecttableView->setColumnWidth(10,50);
    ui->selecttableView->setColumnWidth(11,50);
    ui->selecttableView->setColumnWidth(12,80);
    ui->selecttableView->setAlternatingRowColors(true);
}

void UserMainScn::initQuesModel()
{
    checkBoxList.clear();
    quesModel = new QSqlTableModel(this);
    quesModel->setTable("queslist");
    quesModel->select();
    quesModel->setHeaderData(0,Qt::Orientation::Horizontal,"编号");
    quesModel->setHeaderData(1,Qt::Orientation::Horizontal,"题目");
    quesModel->setHeaderData(2,Qt::Orientation::Horizontal,"题型");
    quesModel->setHeaderData(3,Qt::Orientation::Horizontal,"选项1");
    quesModel->setHeaderData(4,Qt::Orientation::Horizontal,"选项2");
    quesModel->setHeaderData(5,Qt::Orientation::Horizontal,"选项3");
    quesModel->setHeaderData(6,Qt::Orientation::Horizontal,"选项4");
    quesModel->setHeaderData(7,Qt::Orientation::Horizontal,"选项5");
    quesModel->setHeaderData(8,Qt::Orientation::Horizontal,"选项6");
    quesModel->setHeaderData(9,Qt::Orientation::Horizontal,"答案");
    quesModel->setHeaderData(10,Qt::Orientation::Horizontal,"难度");
    quesModel->setHeaderData(11,Qt::Orientation::Horizontal,"分数");
    quesModel->setHeaderData(12,Qt::Orientation::Horizontal,"教师");
    ui->questableView->setModel(quesModel);
    ui->questableView->verticalHeader()->hide();
    ui->questableView->setColumnWidth(0,40);
    ui->questableView->setColumnWidth(1,200);
    ui->questableView->setColumnWidth(2,50);
    ui->questableView->setColumnWidth(3,80);
    ui->questableView->setColumnWidth(4,80);
    ui->questableView->setColumnWidth(5,80);
    ui->questableView->setColumnWidth(6,80);
    ui->questableView->setColumnWidth(7,80);
    ui->questableView->setColumnWidth(8,80);
    ui->questableView->setColumnWidth(9,80);
    ui->questableView->setColumnWidth(10,50);
    ui->questableView->setColumnWidth(11,50);
    ui->questableView->setColumnWidth(12,80);
    ui->questableView->setAlternatingRowColors(true);
    for (int i = 0; i < quesModel->rowCount(); ++i) {
        QCheckBox *checkBox = new QCheckBox(quesModel->index(i,0).data().toString());
        ui->questableView->setIndexWidget(quesModel->index(i,0),checkBox);
        checkBoxList.append(checkBox);
    }
}

void UserMainScn::initSelectExamListModel()
{
    examModel = new QSqlTableModel(this);
    examModel->setTable("examlist");
    examModel->select();
    examModel->setHeaderData(0,Qt::Orientation::Horizontal,"试卷编号");
    examModel->setHeaderData(1,Qt::Orientation::Horizontal,"试卷名称");
    examModel->setHeaderData(2,Qt::Orientation::Horizontal,"单选题数量");
    examModel->setHeaderData(3,Qt::Orientation::Horizontal,"多选题数量");
    examModel->setHeaderData(4,Qt::Orientation::Horizontal,"判断题数量");
    examModel->setHeaderData(5,Qt::Orientation::Horizontal,"单选题总分");
    examModel->setHeaderData(6,Qt::Orientation::Horizontal,"多选题总分");
    examModel->setHeaderData(7,Qt::Orientation::Horizontal,"判断题总分");
    examModel->setHeaderData(8,Qt::Orientation::Horizontal,"总题目数量");
    examModel->setHeaderData(9,Qt::Orientation::Horizontal,"总计分数");
    examModel->setHeaderData(10,Qt::Orientation::Horizontal,"所属教师");
    examModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->examtableView->setModel(examModel);
    ui->examtableView->verticalHeader()->hide();
    ui->examtableView->setAlternatingRowColors(true);
}

void UserMainScn::createExamListJson(QString title, QString type, QString c1, QString c2, QString c3, QString c4, QString c5, QString c6, QString result, QString score)
{
    QJsonObject jsonObject;
    jsonObject.insert("title",title);
    jsonObject.insert("type",type);
    jsonObject.insert("c1",c1);
    jsonObject.insert("c2",c2);
    jsonObject.insert("c3",c3);
    jsonObject.insert("c4",c4);
    jsonObject.insert("c5",c5);
    jsonObject.insert("c6",c6);
    jsonObject.insert("result",result);
    jsonObject.insert("score",score);
    quesJsonArray.append(jsonObject);
}

void UserMainScn::createExamInformationJson(int singleCount, int muchCount, int tofCount, int singleScore, int muchScore, int tofScore)
{
    QJsonObject jsonObject;
    jsonObject.insert("singleNumber",QString::number(singleCount));
    jsonObject.insert("muchNumber",QString::number(muchCount));
    jsonObject.insert("tofNumber",QString::number(tofCount));
    jsonObject.insert("singleScore",QString::number(singleScore));
    jsonObject.insert("muchScore",QString::number(muchScore));
    jsonObject.insert("tofScore",QString::number(tofScore));
    jsonObject.insert("allNumber",QString::number(singleCount+muchCount+tofCount));
    jsonObject.insert("allScore",QString::number(singleScore+muchScore+tofScore));
    jsonObject.insert("teacher",currUserName);
    jsonObject.insert("examName",QString::number(qrand()%9999999));
    quesJsonArray.append(jsonObject);
}

void UserMainScn::on_btnDeleteSelectModel_clicked()
{
    QJsonObject jsonObject;
    jsonObject.insert("title",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),1).data().toString());
    jsonObject.insert("type",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),2).data().toString());
    jsonObject.insert("c1",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),3).data().toString());
    jsonObject.insert("c2",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),4).data().toString());
    jsonObject.insert("c3",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),5).data().toString());
    jsonObject.insert("c4",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),6).data().toString());
    jsonObject.insert("c5",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),7).data().toString());
    jsonObject.insert("c6",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),8).data().toString());
    jsonObject.insert("result",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),9).data().toString());
    jsonObject.insert("level",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),10).data().toString());
    jsonObject.insert("score",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),11).data().toString());
    jsonObject.insert("teacher",ui->selecttableView->model()->index(ui->selecttableView->currentIndex().row(),12).data().toString());
    if(tcpClient->sendMsg("removequestion",jsonObject)){
        ui->selecttableView->model()->removeRow(ui->selecttableView->currentIndex().row());
        QMessageBox::information(nullptr,"删除","服务器删除试题完成!");
    }
}

void UserMainScn::on_btnGoBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserMainScn::on_btnQuesExportExcel_clicked()
{
    QXlsx::Document xlsx;
    QXlsx::Format format;
    format.setFont(QFont("黑体"));
    format.setFontSize(10);
    format.setFontBold(true);
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format.setVerticalAlignment(QXlsx::Format::AlignVCenter);

    xlsx.write(1,1,"试题编号",format);
    xlsx.write(1,2,"题目",format);
    xlsx.write(1,3,"题型",format);
    xlsx.write(1,4,"选项1",format);
    xlsx.write(1,5,"选项2",format);
    xlsx.write(1,6,"选项3",format);
    xlsx.write(1,7,"选项4",format);
    xlsx.write(1,8,"选项5",format);
    xlsx.write(1,9,"选项6",format);
    xlsx.write(1,10,"答案",format);
    xlsx.write(1,11,"难度等级",format);
    xlsx.write(1,12,"分数",format);
    xlsx.write(1,13,"教师",format);

    for (int i = 1; i < 8; ++i) {
        xlsx.setColumnWidth(i,20);
    }

    QXlsx::Format format2;
    format2.setFont(QFont("黑体"));
    format2.setFontSize(9);
    format2.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format2.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    QString saveFileName = QFileDialog::getSaveFileName(this,nullptr,nullptr,"Excel工作表(*.xlsx)");
    if(saveFileName != ""){
        for (int i = 0; i < selectModel->rowCount(); ++i) {
            for (int j = 0; j < selectModel->columnCount(); ++j) {
                QString data = selectModel->index(i,j).data().toString();
                xlsx.write(i+2,j+1,data,format2);
            }
        }
        xlsx.saveAs(saveFileName);
    }
}

void UserMainScn::on_btnQuesNone_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserMainScn::on_btnSelectQuesModel_clicked()
{
    QString text = ui->leSelectQues->text();
    if(quesModel){
        quesModel->setFilter(QObject::tr("ex_title like '%%1%'").arg(text));
        quesModel->select();
    }
}

void UserMainScn::on_btnAddExam_clicked()
{
    initQuesModel();
    ui->stackedWidget->setCurrentIndex(5);
}

void UserMainScn::on_btnQuesCommit_clicked()
{
    quesJsonArray = QJsonArray();
    int singleCount = 0;
    int muchCount = 0;
    int tofCount = 0;
    int singleScore = 0;
    int muchScore = 0;
    int tofScore = 0;
    for (int i = 0; i < checkBoxList.size(); ++i) {
        if(checkBoxList.value(i)->isChecked()){
            QString title = quesModel->index(i,1).data().toString();
            QString type = quesModel->index(i,2).data().toString();
            QString c1 = quesModel->index(i,3).data().toString();
            QString c2 = quesModel->index(i,4).data().toString();
            QString c3 = quesModel->index(i,5).data().toString();
            QString c4 = quesModel->index(i,6).data().toString();
            QString c5 = quesModel->index(i,7).data().toString();
            QString c6 = quesModel->index(i,8).data().toString();
            QString result = quesModel->index(i,9).data().toString();
            QString score = quesModel->index(i,11).data().toString();
            createExamListJson(title,type,c1,c2,c3,c4,c5,c6,result,score);
            if(type == "单选"){
                singleCount++;
                singleScore += score.toInt();
            }
            if(type == "多选"){
                muchCount++;
                muchScore += score.toInt();
            }
            if(type == "判断"){
                tofCount++;
                tofScore += score.toInt();
            }
        }
    }
    ui->text_examsingle->setText(QString::number(singleCount));
    ui->text_exammuch->setText(QString::number(muchCount));
    ui->text_examtof->setText(QString::number(tofCount));
    ui->text_examall->setText(QString::number(singleCount+muchCount+tofCount));
    ui->text_scoresingle->setText(QString::number(singleScore));
    ui->text_scoremuch->setText(QString::number(muchScore));
    ui->text_scoretof->setText(QString::number(tofScore));
    ui->text_scoreall->setText(QString::number(singleScore+muchScore+tofScore));
    createExamInformationJson(singleCount,muchCount,tofCount,singleScore,muchScore,tofScore);
    ui->stackedWidget->setCurrentIndex(6);
}

void UserMainScn::on_btnExamReturn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserMainScn::on_btnAddExamCommit_clicked()
{
    QString examName = ui->leExamName->text();
    if(!examName.isEmpty()){
        if(ui->cbChoiceRand->isChecked()){
            initRandQuesArray();
        }
        QJsonObject nameJson = quesJsonArray.last().toObject();
        nameJson.insert("examName",examName);
        quesJsonArray.removeLast();
        quesJsonArray.append(nameJson);
        if(tcpClient->sendMsg("addexam",quesJsonArray)){
            QMessageBox::information(nullptr,"上传试卷","上传成功!");
            ui->stackedWidget->setCurrentIndex(0);
        }else{
            QMessageBox::information(nullptr,"上传试卷","上传失败，请检查网络");
        }
    }else{
        QMessageBox::information(nullptr,"上传试卷","上传失败，试卷名称不能为空");
    }
}

void UserMainScn::on_btnPreviewReturn_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
}

void UserMainScn::on_singleSpinBox_valueChanged(int arg1)
{
    ui->allSpinBox->setValue(ui->singleSpinBox->value()+ui->muchSpinBox->value()+ui->tofSpinBox->value());
}

void UserMainScn::on_muchSpinBox_valueChanged(int arg1)
{
    ui->allSpinBox->setValue(ui->singleSpinBox->value()+ui->muchSpinBox->value()+ui->tofSpinBox->value());
}

void UserMainScn::on_tofSpinBox_valueChanged(int arg1)
{
    ui->allSpinBox->setValue(ui->singleSpinBox->value()+ui->muchSpinBox->value()+ui->tofSpinBox->value());
}

void UserMainScn::on_btnAddRandExam_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void UserMainScn::on_btnExamRandReturn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserMainScn::on_btnRandExam_clicked()
{
    if(ui->btnRandExam->text() == "开始组卷"){
        QString examName = ui->leExamRandName->text();
        if (examName != "") {
            QJsonObject jsonObject;
            jsonObject.insert("singleNumber",QString::number(ui->singleSpinBox->value()));
            jsonObject.insert("muchNumber",QString::number(ui->muchSpinBox->value()));
            jsonObject.insert("tofNumber",QString::number(ui->tofSpinBox->value()));
            jsonObject.insert("level",QString::number(ui->levelSpinBox->value()));
            jsonObject.insert("examName",examName);
            jsonObject.insert("teacher",currUserName);
            if(ui->cbChoiceRandRand->isChecked()){
                jsonObject.insert("isRand",QString("1"));
            }else{
                jsonObject.insert("isRand",QString("0"));
            }
            if(tcpClient->sendMsg("addrandexam",jsonObject)){
                QMessageBox::information(nullptr,"上传试卷","随机组卷成功，试卷已保存");
            }else{
                QMessageBox::information(nullptr,"上传试卷","组卷失败，请检查网络连接");
            }
        } else {
            QMessageBox::information(nullptr,"上传试卷","上传失败，试卷名称不能为空");
        }
    }
}

void UserMainScn::on_btnExcelImportExam_clicked()
{
    QString saveFileName = QFileDialog::getOpenFileName(this,nullptr,nullptr,"Excel工作表(*.xlsx)");
    if(saveFileName != ""){
        int singleCount = 0;
        int muchCount = 0;
        int tofCount = 0;
        int singleScore = 0;
        int muchScore = 0;
        int tofScore = 0;
        QXlsx::Document xlsx(saveFileName);
        int rowCount = 2;
        quesJsonArray = QJsonArray();
        while (xlsx.read(rowCount,1).toString() != "") {
            QString title = xlsx.read(rowCount,1).toString();
            QString type = xlsx.read(rowCount,2).toString();
            QString c1 = xlsx.read(rowCount,3).toString();
            QString c2 = xlsx.read(rowCount,4).toString();
            QString c3 = xlsx.read(rowCount,5).toString();
            QString c4 = xlsx.read(rowCount,6).toString();
            QString c5 = xlsx.read(rowCount,7).toString();
            QString c6 = xlsx.read(rowCount,8).toString();
            QString result = xlsx.read(rowCount,9).toString();
            QString score = xlsx.read(rowCount,11).toString();
            createExamListJson(title,type,c1,c2,c3,c4,c5,c6,result,score);
            if(type == "单选"){
                singleCount++;
                singleScore += score.toInt();
            }
            if(type == "多选"){
                muchCount++;
                muchScore += score.toInt();
            }
            if(type == "判断"){
                tofCount++;
                tofScore += score.toInt();
            }
            rowCount++;
        }
        createExamInformationJson(singleCount,muchCount,tofCount,singleScore,muchScore,tofScore);
        if(tcpClient->sendMsg("addexam",quesJsonArray)){
            QMessageBox::information(nullptr,"上传试卷","上传成功!");
        }else{
            QMessageBox::information(nullptr,"上传试卷","上传失败，请检查网络");
        }
    }
}

void UserMainScn::on_btnGoBackExamSelect_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserMainScn::on_btnSelectExam_clicked()
{
    QString text = ui->leSelectExam->text();
    if(examModel){
        examModel->setFilter(QObject::tr("(examid like '%%1%') or (examname like '%%1%')").arg(text));
        examModel->select();
    }
}

void UserMainScn::on_btnDeleteSelectExam_clicked()
{
    QString examid = examModel->index(ui->examtableView->currentIndex().row(),0).data().toString();
    if(examid != ""){
        examModel->removeRow(ui->examtableView->currentIndex().row());
        int ret = QMessageBox::information(this,"删除试卷","确认删除编号"+examid+"试卷吗？",QMessageBox::Yes,QMessageBox::No);
        if(ret == QMessageBox::Yes){
            examModel->submitAll();
            QJsonObject jsonObject;
            jsonObject.insert("examid",examid);
            tcpClient->sendMsg("removeexamlist",jsonObject);
        }else{
            examModel->revertAll();
        }
    }
}

void UserMainScn::on_btnShowExam_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void UserMainScn::on_btnShowExamInfo_clicked()
{
    QString examid = examModel->index(ui->examtableView->currentIndex().row(),0).data().toString();
    if(examid != ""){
        QJsonObject jsonObject;
        jsonObject.insert("examid",examid);
        tcpClient->sendMsg("getexam",jsonObject);
    }
}

void UserMainScn::on_btnStartExam_clicked()
{
    QString examid = ui->leExamid->text();
    QString startTime = ui->dateTimeEditStart->text();
    QString stopTime = ui->dateTimeEditStop->text();
    QString school = ui->leSchool->text();
    QString classid = ui->leClassId->text();
    if(!examid.isEmpty()&&!school.isEmpty()&&!classid.isEmpty()){
        QJsonObject jsonObject;
        jsonObject.insert("examid",examid);
        jsonObject.insert("starttime",startTime);
        jsonObject.insert("stoptime",stopTime);
        jsonObject.insert("school",school);
        jsonObject.insert("classid",classid);
        jsonObject.insert("teacher",currUserName);
        if(tcpClient->sendMsg("commitexam",jsonObject)){
            QMessageBox::information(this,"发布完成","发布成功!");
        }else{
            QMessageBox::information(this,"发布失败","网络连接异常");
        }
    }else{
        QMessageBox::information(this,"发布失败","请确认信息填写完整");
    }
}

void UserMainScn::on_btnSelectExamLogModel_clicked()
{
    QString text = ui->leExamLogSelect->text();
    examlogModel->setFilter(QObject::tr("(username like '%%1%')").arg(text));
    examlogModel->select();
}

void UserMainScn::on_btnErrorReset_clicked()
{
    QString text = ui->leExamLogSelect->text();
    examlogModel->setFilter(QObject::tr("(examname like '%%1%')").arg(text));
    examlogModel->select();
}

void UserMainScn::on_btnExamLogExportExcel_clicked()
{
    QXlsx::Document xlsx;
    QXlsx::Format format;
    format.setFont(QFont("黑体"));
    format.setFontSize(10);
    format.setFontBold(true);
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format.setVerticalAlignment(QXlsx::Format::AlignVCenter);

    xlsx.write(1,1,"试题编号",format);
    xlsx.write(1,2,"试卷名称",format);
    xlsx.write(1,3,"单选数量",format);
    xlsx.write(1,4,"多选数量",format);
    xlsx.write(1,5,"判断数量",format);
    xlsx.write(1,6,"合计数量",format);
    xlsx.write(1,7,"单选得分",format);
    xlsx.write(1,8,"多选得分",format);
    xlsx.write(1,9,"判断得分",format);
    xlsx.write(1,10,"合计得分",format);
    xlsx.write(1,11,"难度等级",format);
    xlsx.write(1,12,"发起教师",format);
    xlsx.write(1,13,"考试用时",format);
    xlsx.write(1,14,"参与考生",format);

    for (int i = 1; i < 15; ++i) {
        xlsx.setColumnWidth(i,20);
    }

    QXlsx::Format format2;
    format2.setFont(QFont("黑体"));
    format2.setFontSize(9);
    format2.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format2.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    QString saveFileName = QFileDialog::getSaveFileName(this,nullptr,nullptr,"Excel工作表(*.xlsx)");
    if(saveFileName != ""){
        for (int i = 0; i < examlogModel->rowCount(); ++i) {
            for (int j = 0; j < examlogModel->columnCount(); ++j) {
                QString data = examlogModel->index(i,j).data().toString();
                xlsx.write(i+2,j+1,data,format2);
            }
        }
        xlsx.saveAs(saveFileName);
    }
}

void UserMainScn::initExamLogModel()
{
    examlogModel = new QSqlTableModel(this);
    examlogModel->setTable("examlog");
    examlogModel->setFilter(QObject::tr("username = '%1'").arg(currUserName));
    examlogModel->select();
    examlogModel->setHeaderData(0,Qt::Orientation::Horizontal,"试卷编号");
    examlogModel->setHeaderData(1,Qt::Orientation::Horizontal,"试卷名称");
    examlogModel->setHeaderData(2,Qt::Orientation::Horizontal,"单选数量");
    examlogModel->setHeaderData(3,Qt::Orientation::Horizontal,"多选数量");
    examlogModel->setHeaderData(4,Qt::Orientation::Horizontal,"判断数量");
    examlogModel->setHeaderData(5,Qt::Orientation::Horizontal,"合计数量");
    examlogModel->setHeaderData(6,Qt::Orientation::Horizontal,"单选得分");
    examlogModel->setHeaderData(7,Qt::Orientation::Horizontal,"多选得分");
    examlogModel->setHeaderData(8,Qt::Orientation::Horizontal,"判断得分");
    examlogModel->setHeaderData(9,Qt::Orientation::Horizontal,"合计得分");
    examlogModel->setHeaderData(10,Qt::Orientation::Horizontal,"发起教师");
    examlogModel->setHeaderData(11,Qt::Orientation::Horizontal,"考试用时");
    examlogModel->setHeaderData(12,Qt::Orientation::Horizontal,"参与考生");
    ui->examlogtableView->setModel(examlogModel);
    ui->examlogtableView->verticalHeader()->hide();
    ui->examlogtableView->setColumnWidth(0,80);
    ui->examlogtableView->setColumnWidth(1,80);
    ui->examlogtableView->setColumnWidth(2,80);
    ui->examlogtableView->setColumnWidth(3,80);
    ui->examlogtableView->setColumnWidth(4,80);
    ui->examlogtableView->setColumnWidth(5,80);
    ui->examlogtableView->setColumnWidth(6,80);
    ui->examlogtableView->setColumnWidth(7,80);
    ui->examlogtableView->setColumnWidth(8,80);
    ui->examlogtableView->setColumnWidth(9,80);
    ui->examlogtableView->setColumnWidth(10,80);
    ui->examlogtableView->setColumnWidth(11,80);
    ui->examlogtableView->setColumnWidth(12,80);
    ui->examlogtableView->setAlternatingRowColors(true);
}

void UserMainScn::on_btnSetUserData_clicked()
{
    QString username = ui->leUser->text();
    QString password = ui->lePass->text();
    QString phone = ui->lePhone->text();
    QString school = ui->leSchoolName->text();
    QString classid = ui->leClassId->text();
    QString question = ui->leQues->text();
    QString result = ui->leResult->text();
    QJsonObject jsonObject;
    jsonObject.insert("username",username);
    jsonObject.insert("password",password);
    jsonObject.insert("phone",phone);
    jsonObject.insert("school",school);
    jsonObject.insert("class",classid);
    jsonObject.insert("question",question);
    jsonObject.insert("result",result);
    jsonObject.insert("role",QString("teacher"));
    if(tcpClient->sendMsg("resetuser",jsonObject)){
        QMessageBox::information(nullptr,"修改个人资料","修改成功，信息已提交至服务器");
    }else{
        QMessageBox::information(nullptr,"修改个人资料","修改失败");
    }
}

void UserMainScn::on_btnCloseSys_clicked()
{
    this->close();
}
