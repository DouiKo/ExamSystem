#include "clientmsg.h"

extern QSqlQuery outQuery;
extern QSqlQuery inQuery;

ClientMsg::ClientMsg(QByteArray readData, QTcpSocket *socket, QObject *parent) : QObject(parent)
{
    QDataStream in(&readData,QIODevice::ReadOnly);
    QString msgType;
    in >> msgType;
    if(msgType == "login"){
        receiveLoginMsg(readData,socket);
    }
    if(msgType == "reg"){
        receiveRegMsg(readData,socket);
    }
    if(msgType == "addquestion"){
        receiveAddQuestionMsg(readData);
    }
    if(msgType == "getquestion"){
        receiveGetQuestionMsg(readData,socket);
    }
    if(msgType == "removequestion"){
        receiveRemoveQuestionMsg(readData);
    }
    if(msgType == "addexam"){
        receiveAddNewExam(readData);
    }
    if(msgType == "addrandexam"){
        receiveAddRandExam(readData,socket);
    }
    if(msgType == "getexamlist"){
        receiveAddGetExamList(readData,socket);
    }
    if(msgType == "removeexamlist"){
        receiveRemoveExamList(readData);
    }
    if(msgType == "getexam"){
        receiveGetExam(readData,socket);
    }
    if(msgType == "getfreeexam"){
        receiveGetFreeExam(readData,socket);
    }
    if(msgType == "commitexam"){
        receiveCommitExam(readData);
    }
    if(msgType == "searchexam"){
        receiveSearchExam(readData,socket);
    }
    if(msgType == "billexam"){
        receiveBillExamMsg(readData);
    }
    if(msgType == "getexamlog"){
        receiveGetExamLogMsg(readData,socket);
    }
    if(msgType == "resetuser"){
        receiveResetUserMsg(readData);
    }
    if(msgType == "userbackup"){
        receiveUserBackupMsg(readData,socket);
    }
}

bool ClientMsg::sendMsg(QString type, QJsonObject data, QTcpSocket *socket)
{
    QByteArray byteArray;
    QDataStream in(&byteArray,QIODevice::WriteOnly);
    in << type << data;
    if(socket){
        socket->write(byteArray);
        return true;
    }else{
        return false;
    }
}

bool ClientMsg::sendMsg(QString type, QJsonArray array, QTcpSocket *socket)
{
    QByteArray byteArray;
    QDataStream in(&byteArray,QIODevice::WriteOnly);
    in << type << array;
    if(socket){
        socket->write(byteArray);
        return true;
    }else{
        return false;
    }
}

QJsonObject ClientMsg::studentLogin(QJsonObject jsonObject)
{
    QJsonObject sendJson;
    outQuery.exec("SELECT * FROM student WHERE ex_username = '"+jsonObject.value("username").toString()+"'");
    if(outQuery.next()){
        if(jsonObject.value("password").toString() == outQuery.value(1).toString()){
            sendJson.insert("error","none");
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
            sendJson.insert("phone",outQuery.value(2).toString());
            sendJson.insert("school",outQuery.value(3).toString());
            sendJson.insert("class",outQuery.value(4).toString());
            sendJson.insert("question",outQuery.value(6).toString());
            sendJson.insert("result",outQuery.value(7).toString());
        }else{
            sendJson.insert("error","passerror");
        }
    }else{
        sendJson.insert("error","nouser");
    }
    return sendJson;
}

QJsonObject ClientMsg::teacherLogin(QJsonObject jsonObject)
{
    QJsonObject sendJson;
    outQuery.exec("SELECT * FROM teacher WHERE ex_username = '"+jsonObject.value("username").toString()+"'");
    if(outQuery.next()){
        if(jsonObject.value("password").toString() == outQuery.value(1).toString()){
            sendJson.insert("error","none");
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
            sendJson.insert("phone",outQuery.value(2).toString());
            sendJson.insert("school",outQuery.value(3).toString());
            sendJson.insert("class",outQuery.value(4).toString());
            sendJson.insert("question",outQuery.value(5).toString());
            sendJson.insert("result",outQuery.value(6).toString());
        }else{
            sendJson.insert("error","passerror");
        }
    }else{
        sendJson.insert("error","nouser");
    }
    return sendJson;
}

QJsonObject ClientMsg::studentBackup(QJsonObject jsonObject)
{
    QJsonObject sendJson;
    int msgCount = 0;
    if(jsonObject.value("username").toString() != ""){
        outQuery.exec("SELECT * FROM student WHERE ex_username = '"+jsonObject.value("username").toString()+"'");
        if(outQuery.next()){
            msgCount++;
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
        }
    }
    if(jsonObject.value("phone").toString() != ""){
        outQuery.exec("SELECT * FROM student WHERE ex_phone = '"+jsonObject.value("phone").toString()+"' AND ex_username = '"+jsonObject.value("username").toString()+"'");
        if(outQuery.next()){
            msgCount++;
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
        }
    }
    if(jsonObject.value("school").toString() != ""){
        outQuery.exec("SELECT * FROM student WHERE ex_school = '"+jsonObject.value("school").toString()+"' AND ex_username = '"+jsonObject.value("username").toString()+"'");
        if(outQuery.next()){
            msgCount++;
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
        }
    }
    if(jsonObject.value("class").toString() != ""){
        outQuery.exec("SELECT * FROM student WHERE ex_class = '"+jsonObject.value("class").toString()+"' AND ex_username = '"+jsonObject.value("username").toString()+"'");
        if(outQuery.next()){
            msgCount++;
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
        }
    }
    if(jsonObject.value("result").toString() != ""){
        outQuery.exec("SELECT * FROM student WHERE ex_result = '"+jsonObject.value("result").toString()+"' AND ex_username = '"+jsonObject.value("username").toString()+"'");
        if(outQuery.next()){
            msgCount++;
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
        }
    }
    if(msgCount >= 3){
        sendJson.insert("error","noerror");
    }else{
        sendJson.insert("error","error");
    }
    return sendJson;
}

QJsonObject ClientMsg::teacherBackup(QJsonObject jsonObject)
{
    QJsonObject sendJson;
    int msgCount = 0;
    if(jsonObject.value("username").toString() != ""){
        outQuery.exec("SELECT * FROM teacher WHERE ex_username = '"+jsonObject.value("username").toString()+"'");
        if(outQuery.next()){
            msgCount++;
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
        }
    }
    if(jsonObject.value("phone").toString() != ""){
        outQuery.exec("SELECT * FROM teacher WHERE ex_phone = '"+jsonObject.value("phone").toString()+"' AND ex_username = '"+jsonObject.value("username").toString()+"'");
        if(outQuery.next()){
            msgCount++;
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
        }
    }
    if(jsonObject.value("school").toString() != ""){
        outQuery.exec("SELECT * FROM teacher WHERE ex_school = '"+jsonObject.value("school").toString()+"' AND ex_username = '"+jsonObject.value("username").toString()+"'");
        if(outQuery.next()){
            msgCount++;
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
        }
    }
    if(jsonObject.value("class").toString() != ""){
        outQuery.exec("SELECT * FROM teacher WHERE ex_class = '"+jsonObject.value("class").toString()+"' AND ex_username = '"+jsonObject.value("username").toString()+"'");
        if(outQuery.next()){
            msgCount++;
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
        }
    }
    if(jsonObject.value("result").toString() != ""){
        outQuery.exec("SELECT * FROM teacher WHERE ex_result = '"+jsonObject.value("result").toString()+"' AND ex_username = '"+jsonObject.value("username").toString()+"'");
        if(outQuery.next()){
            msgCount++;
            sendJson.insert("username",outQuery.value(0).toString());
            sendJson.insert("password",outQuery.value(1).toString());
        }
    }
    if(msgCount >= 3){
        sendJson.insert("error","noerror");
    }else{
        sendJson.insert("error","error");
    }
    return sendJson;
}

QJsonObject ClientMsg::studentReg(QJsonObject jsonObject)
{
    QJsonObject sendJson;
    QString username = jsonObject.value("username").toString();
    QString password = jsonObject.value("password").toString();
    QString phone = jsonObject.value("phonenumber").toString();
    QString claid = jsonObject.value("classid").toString();
    QString stuid = jsonObject.value("studentid").toString();
    QString schname = jsonObject.value("schoolname").toString();
    QString ques = jsonObject.value("question").toString();
    QString result = jsonObject.value("result").toString();

    bool isInsert = inQuery.exec("INSERT INTO student (ex_username,ex_password,ex_phone,ex_school,ex_class,ex_student,ex_ques,ex_result,ex_state)"
                                 "VALUES('"+username+"','"+password+"','"+phone+"','"+schname+"','"+claid+"','"+stuid+"','"+ques+"','"+result+"','0')");
    if(isInsert){
        sendJson.insert("error","none");
    }else{
        sendJson.insert("error","failure");
    }
    return sendJson;
}

QJsonObject ClientMsg::teacherReg(QJsonObject jsonObject)
{
    QJsonObject sendJson;
    QString username = jsonObject.value("username").toString();
    QString password = jsonObject.value("password").toString();
    QString phone = jsonObject.value("phonenumber").toString();
    QString claid = jsonObject.value("classid").toString();
    QString schname = jsonObject.value("schoolname").toString();
    QString ques = jsonObject.value("question").toString();
    QString result = jsonObject.value("result").toString();
    bool isInsert = inQuery.exec("INSERT INTO teacher (ex_username,ex_password,ex_phone,ex_school,ex_class,ex_ques,ex_result,ex_state)"
                                 "VALUES('"+username+"','"+password+"','"+phone+"','"+schname+"','"+claid+"','"+ques+"','"+result+"','0')");
    if(isInsert){
        sendJson.insert("error","none");
    }else{
        sendJson.insert("error","failure");
    }
    return sendJson;
}

QJsonObject ClientMsg::createQuesJson(QString title, QString type, QString c1, QString c2, QString c3, QString c4, QString c5, QString c6, QString result, QString level, QString score, QString teacher)
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
    jsonObject.insert("level",level);
    jsonObject.insert("score",score);
    jsonObject.insert("result",result);
    jsonObject.insert("teacher",teacher);
    return jsonObject;
}

void ClientMsg::receiveLoginMsg(QByteArray data, QTcpSocket *socket)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString type,sendUser;
    QJsonObject jsonObject;
    in >> type >>sendUser >> jsonObject;
    qDebug()<<type<<sendUser<<jsonObject;
    if(jsonObject.value("role") == "student"){
        sendMsg("logindone",studentLogin(jsonObject),socket);
    }
    if(jsonObject.value("role") == "teacher"){
        sendMsg("logindone",teacherLogin(jsonObject),socket);
    }
}

void ClientMsg::receiveRegMsg(QByteArray data, QTcpSocket *socket)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString type,sendUser;
    QJsonObject jsonObject;
    in >> type >>sendUser >> jsonObject;
    if(jsonObject.value("role") == "student"){
        sendMsg("regdone",studentReg(jsonObject),socket);
    }
    if(jsonObject.value("role") == "teacher"){
        sendMsg("regdone",teacherReg(jsonObject),socket);
    }
}

void ClientMsg::receiveAddQuestionMsg(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString type,sendUser;
    QJsonObject jsonObject;
    in >> type >>sendUser >> jsonObject;
    qDebug()<<jsonObject;
    if(jsonObject.value("type").toString() == "单选"){
        onAddSingleQues(jsonObject);
    }
    if(jsonObject.value("type").toString() == "多选"){
        onAddMuchQues(jsonObject);
    }
    if(jsonObject.value("type").toString() == "判断"){
        onAddTofQues(jsonObject);
    }
}

void ClientMsg::receiveGetQuestionMsg(QByteArray data,QTcpSocket *socket)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString type,sendUser;
    in >> type >>sendUser;
    QJsonArray jsonArray;
    outQuery.exec("SELECT * FROM singlelist WHERE ex_teacher = '"+sendUser+"'");
    while (outQuery.next()) {
        QString title = outQuery.value(1).toString();
        QString type = outQuery.value(2).toString();
        QString c1 = outQuery.value(3).toString();
        QString c2 = outQuery.value(4).toString();
        QString c3 = outQuery.value(5).toString();
        QString c4 = outQuery.value(6).toString();
        QString c5 = outQuery.value(7).toString();
        QString c6 = outQuery.value(8).toString();
        QString result = outQuery.value(9).toString();
        QString level = outQuery.value(10).toString();
        QString score = outQuery.value(11).toString();
        QString teacher = outQuery.value(12).toString();
        jsonArray.append(createQuesJson(title,type,c1,c2,c3,c4,c5,c6,result,level,score,teacher));
    }
    outQuery.exec("SELECT * FROM muchlist WHERE ex_teacher = '"+sendUser+"'");
    while (outQuery.next()) {
        QString title = outQuery.value(1).toString();
        QString type = outQuery.value(2).toString();
        QString c1 = outQuery.value(3).toString();
        QString c2 = outQuery.value(4).toString();
        QString c3 = outQuery.value(5).toString();
        QString c4 = outQuery.value(6).toString();
        QString c5 = outQuery.value(7).toString();
        QString c6 = outQuery.value(8).toString();
        QString result = outQuery.value(9).toString();
        QString level = outQuery.value(10).toString();
        QString score = outQuery.value(11).toString();
        QString teacher = outQuery.value(12).toString();
        jsonArray.append(createQuesJson(title,type,c1,c2,c3,c4,c5,c6,result,level,score,teacher));
    }
    outQuery.exec("SELECT * FROM toflist WHERE ex_teacher = '"+sendUser+"'");
    while (outQuery.next()) {
        QString title = outQuery.value(1).toString();
        QString type = outQuery.value(2).toString();
        QString c1 = outQuery.value(3).toString();
        QString c2 = outQuery.value(4).toString();
        QString result = outQuery.value(5).toString();
        QString level = outQuery.value(6).toString();
        QString score = outQuery.value(7).toString();
        QString teacher = outQuery.value(8).toString();
        jsonArray.append(createQuesJson(title,type,c1,c2,"","","","",result,level,score,teacher));
    }
    sendMsg("getquestion",jsonArray,socket);
}

void ClientMsg::receiveRemoveQuestionMsg(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType,sendUser;
    QJsonObject jsonObject;
    in >> msgType >>sendUser >> jsonObject;
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
    if(type == "单选"){
        inQuery.exec("DELETE FROM singlelist WHERE ex_title = '"+title+"' and ex_type = '"+type+"' and ex_1 = '"+c1+"' and ex_2 = '"+c2+"' and ex_3 = '"+c3+"' and ex_4 = '"+c4+"' and ex_5 = '"+c5+"' and ex_6 = '"+c6+"' and ex_result = '"+result+"' and ex_level = '"+level+"' and ex_score = '"+score+"' and ex_teacher = '"+teacher+"'");
    }
    if(type == "多选"){
        inQuery.exec("DELETE FROM muchlist WHERE ex_title = '"+title+"' and ex_type = '"+type+"' and ex_1 = '"+c1+"' and ex_2 = '"+c2+"' and ex_3 = '"+c3+"' and ex_4 = '"+c4+"' and ex_5 = '"+c5+"' and ex_6 = '"+c6+"' and ex_result = '"+result+"' and ex_level = '"+level+"' and ex_score = '"+score+"' and ex_teacher = '"+teacher+"'");
    }
    if(type == "判断"){
        inQuery.exec("DELETE FROM toflist WHERE ex_title = '"+title+"' and ex_type = '"+type+"' and ex_1 = '"+c1+"' and ex_2 = '"+c2+"' and ex_result = '"+result+"' and ex_level = '"+level+"' and ex_score = '"+score+"' and ex_teacher = '"+teacher+"'");
    }
}

void ClientMsg::receiveAddNewExam(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType,sendUser;
    QJsonArray jsonArray;
    in >> msgType >>sendUser >> jsonArray;
    insertExamToDataBaseAndSaveAsJson(jsonArray);
}

void ClientMsg::receiveAddRandExam(QByteArray data, QTcpSocket *socket)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType,sendUser;
    QJsonObject jsonObject;
    in >> msgType >>sendUser >> jsonObject;
    int singleNumber = jsonObject.value("singleNumber").toString().toInt();
    int muchNumber = jsonObject.value("muchNumber").toString().toInt();
    int tofNumber = jsonObject.value("tofNumber").toString().toInt();
    QString level = jsonObject.value("level").toString();
    QString examName = jsonObject.value("examName").toString();
    QString teacher = jsonObject.value("teacher").toString();
    QString isRand = jsonObject.value("isRand").toString();
    QJsonArray jsonArray = createRandQuesJson(singleNumber,muchNumber,tofNumber,level,examName,teacher,isRand,socket);
    insertExamToDataBaseAndSaveAsJson(jsonArray);
}

void ClientMsg::receiveAddGetExamList(QByteArray data, QTcpSocket *socket)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType,sendUser;
    QJsonObject jsonObject;
    in >> msgType >>sendUser >> jsonObject;
    QJsonArray jsonArray;
    outQuery.exec("SELECT * FROM examlist WHERE ex_teacher = '"+sendUser+"'");
    while (outQuery.next()) {
        QJsonObject jsonObject;
        jsonObject.insert("examid",outQuery.value(0).toString());
        jsonObject.insert("examname",outQuery.value(1).toString());
        jsonObject.insert("singlenumber",outQuery.value(2).toString());
        jsonObject.insert("muchnumber",outQuery.value(3).toString());
        jsonObject.insert("tofnumber",outQuery.value(4).toString());
        jsonObject.insert("singlescore",outQuery.value(5).toString());
        jsonObject.insert("muchscore",outQuery.value(6).toString());
        jsonObject.insert("tofscore",outQuery.value(7).toString());
        jsonObject.insert("allnumber",outQuery.value(8).toString());
        jsonObject.insert("allscore",outQuery.value(9).toString());
        jsonObject.insert("teacher",outQuery.value(10).toString());
        jsonArray.append(jsonObject);
    }
    sendMsg("getexamlist",jsonArray,socket);
}

void ClientMsg::receiveRemoveExamList(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType,sendUser;
    QJsonObject jsonObject;
    in >> msgType >>sendUser >> jsonObject;
    QString examid = jsonObject.value("examid").toString();
    if(!examid.isEmpty()){
        outQuery.exec("DELETE FROM examlist WHERE examid = '"+examid+"'");
    }
}

void ClientMsg::receiveGetExam(QByteArray data, QTcpSocket *socket)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType,sendUser;
    QJsonObject jsonObject;
    in >> msgType >>sendUser >> jsonObject;
    QString examid = jsonObject.value("examid").toString();
    QJsonArray jsonArray;
    if(!examid.isEmpty()){
        QFile file(examid+".json");
        file.open(QIODevice::ReadOnly);
        QByteArray readFile = file.readAll();
        file.close();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(readFile);
        sendMsg("getexam",jsonDocument.array(),socket);
    }
}

void ClientMsg::receiveGetFreeExam(QByteArray data, QTcpSocket *socket)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType,sendUser;
    QJsonObject jsonObject;
    in >> msgType >>sendUser >> jsonObject;
    QString examid = jsonObject.value("examid").toString();
    QJsonArray jsonArray;
    if(!examid.isEmpty()){
        QFile file(examid+".json");
        file.open(QIODevice::ReadOnly);
        QByteArray readFile = file.readAll();
        file.close();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(readFile);
        sendMsg("getfreeexam",jsonDocument.array(),socket);
    }
}

void ClientMsg::receiveCommitExam(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType,sendUser;
    QJsonObject jsonObject;
    in >> msgType >>sendUser >> jsonObject;
    QString examid = jsonObject.value("examid").toString();
    QString startTime = jsonObject.value("starttime").toString();
    QString stopTime = jsonObject.value("stoptime").toString();
    QString school = jsonObject.value("school").toString();
    QString classid = jsonObject.value("classid").toString();
    QString teacher = jsonObject.value("teacher").toString();
    qDebug()<<jsonObject;
    inQuery.exec("INSERT INTO exam_record (examid,starttime,stoptime,school,class,teacher)VALUES"
                 "('"+examid+"','"+startTime+"','"+stopTime+"','"+school+"','"+classid+"','"+teacher+"')");
    qDebug()<<inQuery.lastError().text();
}

void ClientMsg::receiveSearchExam(QByteArray data, QTcpSocket *socket)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType,sendUser;
    QJsonObject jsonObject;
    in >> msgType >>sendUser >> jsonObject;
    QString school = jsonObject.value("school").toString();
    QString classid = jsonObject.value("classid").toString();
    QJsonArray jsonArray;
    outQuery.exec("SELECT * FROM exam_record WHERE school = '"+school+"' AND class = '"+classid+"'");
    while (outQuery.next()) {
        QJsonObject queryJson;
        queryJson.insert("examid",outQuery.value(0).toString());
        queryJson.insert("starttime",outQuery.value(1).toString());
        queryJson.insert("stoptime",outQuery.value(2).toString());
        queryJson.insert("school",outQuery.value(3).toString());
        queryJson.insert("classid",outQuery.value(4).toString());
        queryJson.insert("teacher",outQuery.value(5).toString());
        jsonArray.append(queryJson);
    }
    sendMsg("searchexam",jsonArray,socket);
}

void ClientMsg::receiveBillExamMsg(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType,sendUser;
    QJsonObject jsonObject;
    in >> msgType >>sendUser >> jsonObject;
    QString examid = jsonObject.value("examid").toString();
    QString examName = jsonObject.value("examName").toString();
    QString singleNumber = jsonObject.value("singleNumber").toString();
    QString muchNumber = jsonObject.value("muchNumber").toString();
    QString tofNumber = jsonObject.value("tofNumber").toString();
    QString allNumber = jsonObject.value("allNumber").toString();
    QString singleScore = jsonObject.value("singleScore").toString();
    QString muchScore = jsonObject.value("muchScore").toString();
    QString tofScore = jsonObject.value("tofScore").toString();
    QString allScore = jsonObject.value("allScore").toString();
    QString teacher = jsonObject.value("teacher").toString();
    QString usedtime = jsonObject.value("usedtime").toString();
    QString username = jsonObject.value("username").toString();
    inQuery.exec("INSERT INTO examlog (examid,examname,singlenumber,muchnumber,tofnumber,allnumber,singlescore,muchscore,tofscore,allscore,teacher,usedtime,username)VALUES"
                 "('"+examid+"','"+examName+"','"+singleNumber+"','"+muchNumber+"','"+tofNumber+"','"+allNumber+"','"+singleScore+"','"+muchScore+"','"+tofScore+"','"+allScore+"','"+teacher+"','"+usedtime+"','"+username+"')");
}

void ClientMsg::receiveGetExamLogMsg(QByteArray data, QTcpSocket *socket)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString msgType,sendUser;
    QJsonObject jsonObject;
    in >> msgType >>sendUser >> jsonObject;
    QString teacher = jsonObject.value("teacher").toString();
    QJsonArray jsonArray;
    outQuery.exec("SELECT * FROM examlog WHERE teacher = '"+teacher+"'");
    while (outQuery.next()) {
        QJsonObject queryJson;
        queryJson.insert("examid",outQuery.value(0).toString());
        queryJson.insert("examname",outQuery.value(1).toString());
        queryJson.insert("singlenumber",outQuery.value(2).toString());
        queryJson.insert("muchnumber",outQuery.value(3).toString());
        queryJson.insert("tofnumber",outQuery.value(4).toString());
        queryJson.insert("allnumber",outQuery.value(5).toString());
        queryJson.insert("singlescore",outQuery.value(6).toString());
        queryJson.insert("muchscore",outQuery.value(7).toString());
        queryJson.insert("tofscore",outQuery.value(8).toString());
        queryJson.insert("allscore",outQuery.value(9).toString());
        queryJson.insert("teacher",outQuery.value(10).toString());
        queryJson.insert("usedtime",outQuery.value(11).toString());
        queryJson.insert("username",outQuery.value(12).toString());
        jsonArray.append(queryJson);
    }
    sendMsg("getexamlog",jsonArray,socket);
}

void ClientMsg::receiveResetUserMsg(QByteArray data)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString type,sendUser;
    QJsonObject jsonObject;
    in >> type >>sendUser >> jsonObject;
    QString username = jsonObject.value("username").toString();
    QString password = jsonObject.value("password").toString();
    QString phone = jsonObject.value("phone").toString();
    QString school = jsonObject.value("school").toString();
    QString classid = jsonObject.value("classid").toString();
    QString question = jsonObject.value("question").toString();
    QString result = jsonObject.value("result").toString();
    QString role = jsonObject.value("role").toString();
    if(role == "student"){
        inQuery.exec("UPDATE student SET ex_password = '"+password+"' WHERE ex_username = '"+username+"'");
        inQuery.exec("UPDATE student SET ex_phone = '"+phone+"' WHERE ex_username = '"+username+"'");
        inQuery.exec("UPDATE student SET ex_school = '"+school+"' WHERE ex_username = '"+username+"'");
        inQuery.exec("UPDATE student SET ex_class = '"+classid+"' WHERE ex_username = '"+username+"'");
        inQuery.exec("UPDATE student SET ex_ques = '"+question+"' WHERE ex_username = '"+username+"'");
        inQuery.exec("UPDATE student SET ex_result = '"+result+"' WHERE ex_username = '"+username+"'");
    }
    if(role == "teacher"){
        inQuery.exec("UPDATE teacher SET ex_password = '"+password+"' WHERE ex_username = '"+username+"'");
        qDebug()<<inQuery.lastError().text();
        inQuery.exec("UPDATE teacher SET ex_phone = '"+phone+"' WHERE ex_username = '"+username+"'");
        inQuery.exec("UPDATE teacher SET ex_school = '"+school+"' WHERE ex_username = '"+username+"'");
        inQuery.exec("UPDATE teacher SET ex_class = '"+classid+"' WHERE ex_username = '"+username+"'");
        inQuery.exec("UPDATE teacher SET ex_ques = '"+question+"' WHERE ex_username = '"+username+"'");
        inQuery.exec("UPDATE teacher SET ex_result = '"+result+"' WHERE ex_username = '"+username+"'");
    }
}

void ClientMsg::receiveUserBackupMsg(QByteArray data, QTcpSocket *socket)
{
    QDataStream in(&data,QIODevice::ReadOnly);
    QString type,sendUser;
    QJsonObject jsonObject;
    in >> type >>sendUser >> jsonObject;
    qDebug()<<type<<sendUser<<jsonObject;
    if(jsonObject.value("role") == "student"){
        sendMsg("userbackup",studentBackup(jsonObject),socket);
    }
    if(jsonObject.value("role") == "teacher"){
        sendMsg("userbackup",teacherBackup(jsonObject),socket);
    }
}

void ClientMsg::insertExamToDataBaseAndSaveAsJson(QJsonArray jsonArray)
{
    QJsonObject examInfoJson = jsonArray.last().toObject();
    QString examName = examInfoJson.value("examName").toString();
    QString singleNumber = examInfoJson.value("singleNumber").toString();
    QString muchNumber = examInfoJson.value("muchNumber").toString();
    QString tofNumber = examInfoJson.value("tofNumber").toString();
    QString singleScore = examInfoJson.value("singleScore").toString();
    QString muchScore = examInfoJson.value("muchScore").toString();
    QString tofScore = examInfoJson.value("tofScore").toString();
    QString allNumber = examInfoJson.value("allNumber").toString();
    QString allScore = examInfoJson.value("allScore").toString();
    QString teacher = examInfoJson.value("teacher").toString();
    inQuery.exec("ALTER TABLE examlist auto_increment = 10000;");
    inQuery.exec("INSERT INTO examlist (examname,singlenumber,muchnumber,tofnumber,singlescore,muchscore,tofscore,allnumber,allscore,ex_teacher)VALUES"
                 "('"+examName+"','"+singleNumber+"','"+muchNumber+"','"+tofNumber+"','"+singleScore+"','"+muchScore+"','"+tofScore+"','"+allNumber+"','"+allScore+"','"+teacher+"')");
    outQuery.exec("SELECT examid FROM  examlist ORDER BY examid DESC LIMIT 1;");
    if(outQuery.next()){
        QString examId = outQuery.value(0).toString();
        QJsonDocument jsonDocumen(jsonArray);
        QByteArray examJsonData = jsonDocumen.toJson(QJsonDocument::Indented);
        QFile file(examId+".json");
        file.open(QIODevice::WriteOnly);
        file.write(examJsonData);
        file.close();
    }
}

QJsonArray ClientMsg::createRandQuesJson(int singleNumner, int muchNumber, int tofNumber, QString level, QString examName, QString teacher, QString isRand, QTcpSocket *socket)
{
    int singleCount = singleNumner;
    int muchCount = muchNumber;
    int tofCount = tofNumber;
    int singleScore = 0;
    int muchScore = 0;
    int tofScore = 0;
    QJsonArray jsonArray;
    inQuery.exec("TRUNCATE TABLE temp_single;");    //清空sql临时表，临时表用户存储本次需要操作的题目，下同
    inQuery.exec("TRUNCATE TABLE temp_much;");
    inQuery.exec("TRUNCATE TABLE temp_tof;");
    inQuery.exec("INSERT INTO temp_single SELECT * FROM singlelist;");
    inQuery.exec("INSERT INTO temp_much SELECT * FROM muchlist;");
    inQuery.exec("INSERT INTO temp_tof SELECT * FROM toflist;");
    while (singleCount > 0) {
        outQuery.exec("SELECT * FROM temp_single WHERE ex_level = "+level+" AND ex_teacher = "+teacher+" ORDER BY RAND() LIMIT 1;");    //随机获取题目
        if(outQuery.next()){
            QJsonObject jsonObject;
            jsonObject.insert("title",outQuery.value(1).toString());
            jsonObject.insert("type",outQuery.value(2).toString());
            jsonObject.insert("c1",outQuery.value(3).toString());
            jsonObject.insert("c2",outQuery.value(4).toString());
            jsonObject.insert("c3",outQuery.value(5).toString());
            jsonObject.insert("c4",outQuery.value(6).toString());
            jsonObject.insert("c5",outQuery.value(7).toString());
            jsonObject.insert("c6",outQuery.value(8).toString());
            jsonObject.insert("result",outQuery.value(9).toString());
            jsonObject.insert("score",outQuery.value(11).toString());
            outQuery.exec("DELETE FROM temp_single WHERE pid = "+outQuery.value(0).toString()+"");
            singleScore += outQuery.value(11).toString().toInt();
            jsonArray.append(jsonObject);
        }
        singleCount--;
    }
    while (muchCount > 0) {
        outQuery.exec("SELECT * FROM temp_much WHERE ex_level = "+level+" AND ex_teacher = "+teacher+" ORDER BY RAND() LIMIT 1;");    //随机获取题目
        if(outQuery.next()){
            QJsonObject jsonObject;
            jsonObject.insert("title",outQuery.value(1).toString());
            jsonObject.insert("type",outQuery.value(2).toString());
            jsonObject.insert("c1",outQuery.value(3).toString());
            jsonObject.insert("c2",outQuery.value(4).toString());
            jsonObject.insert("c3",outQuery.value(5).toString());
            jsonObject.insert("c4",outQuery.value(6).toString());
            jsonObject.insert("c5",outQuery.value(7).toString());
            jsonObject.insert("c6",outQuery.value(8).toString());
            jsonObject.insert("result",outQuery.value(9).toString());
            jsonObject.insert("score",outQuery.value(11).toString());
            outQuery.exec("DELETE FROM temp_much WHERE pid = "+outQuery.value(0).toString()+"");
            muchScore += outQuery.value(11).toString().toInt();
            jsonArray.append(jsonObject);
        }
        muchCount--;
    }
    while (tofCount > 0) {
        outQuery.exec("SELECT * FROM temp_tof WHERE ex_level = "+level+" AND ex_teacher = "+teacher+" ORDER BY RAND() LIMIT 1;");    //随机获取题目
        qDebug()<<outQuery.lastError().text();
        if(outQuery.next()){
            QJsonObject jsonObject;
            jsonObject.insert("title",outQuery.value(1).toString());
            jsonObject.insert("type",outQuery.value(2).toString());
            jsonObject.insert("c1",outQuery.value(3).toString());
            jsonObject.insert("c2",outQuery.value(4).toString());
            jsonObject.insert("c3","");
            jsonObject.insert("c4","");
            jsonObject.insert("c5","");
            jsonObject.insert("c6","");
            jsonObject.insert("result",outQuery.value(5).toString());
            jsonObject.insert("score",outQuery.value(7).toString());
            outQuery.exec("DELETE FROM temp_tof WHERE pid = "+outQuery.value(0).toString()+"");
            tofScore += outQuery.value(11).toString().toInt();
            jsonArray.append(jsonObject);
        }
        tofCount--;
    }
    if(isRand == "1"){
        int hasChoice = 1;
        for (int i = 0; i < jsonArray.count(); ++i) {
            QJsonObject jsonObject = jsonArray[i].toObject();
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
            jsonArray.replace(i,jsonObject);
        }
    }
    QJsonObject infoJson;
    infoJson.insert("examName",examName);
    infoJson.insert("singleNumber",QString::number(singleNumner));
    infoJson.insert("muchNumber",QString::number(muchNumber));
    infoJson.insert("tofNumber",QString::number(tofNumber));
    infoJson.insert("allNumber",QString::number(singleNumner + muchNumber + tofNumber));
    infoJson.insert("singleScore",QString::number(singleScore));
    infoJson.insert("muchScore",QString::number(muchScore));
    infoJson.insert("tofScore",QString::number(tofScore));
    infoJson.insert("allScore",QString::number(singleScore + muchScore + tofScore));
    sendMsg("addrandexam",infoJson,socket);
    jsonArray.append(infoJson);
    return jsonArray;
}

void ClientMsg::onAddSingleQues(QJsonObject jsonObject)
{
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
    inQuery.exec("ALTER TABLE singlelist auto_increment = 0;");
    inQuery.exec("INSERT INTO singlelist (ex_title,ex_type,ex_1,ex_2,ex_3,ex_4,ex_5,ex_6,ex_result,ex_level,ex_score,ex_teacher)"
                 "VALUES('"+title+"','"+type+"','"+c1+"','"+c2+"','"+c3+"','"+c4+"','"+c5+"','"+c6+"','"+result+"','"+level+"','"+score+"','"+teacher+"')");
}

void ClientMsg::onAddMuchQues(QJsonObject jsonObject)
{
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
    inQuery.exec("ALTER TABLE muchlist auto_increment = 0;");
    inQuery.exec("INSERT INTO muchlist (ex_title,ex_type,ex_1,ex_2,ex_3,ex_4,ex_5,ex_6,ex_result,ex_level,ex_score,ex_teacher)"
                 "VALUES('"+title+"','"+type+"','"+c1+"','"+c2+"','"+c3+"','"+c4+"','"+c5+"','"+c6+"','"+result+"','"+level+"','"+score+"','"+teacher+"')");
}

void ClientMsg::onAddTofQues(QJsonObject jsonObject)
{
    QString title = jsonObject.value("title").toString();
    QString type = jsonObject.value("type").toString();
    QString c1 = jsonObject.value("c1").toString();
    QString c2 = jsonObject.value("c2").toString();
    QString result = jsonObject.value("result").toString();
    QString level = jsonObject.value("level").toString();
    QString score = jsonObject.value("score").toString();
    QString teacher = jsonObject.value("teacher").toString();
    inQuery.exec("ALTER TABLE toflist auto_increment = 0;");
    inQuery.exec("INSERT INTO toflist (ex_title,ex_type,ex_1,ex_2,ex_result,ex_level,ex_score,ex_teacher)"
                 "VALUES('"+title+"','"+type+"','"+c1+"','"+c2+"','"+result+"','"+level+"','"+score+"','"+teacher+"')");
}
