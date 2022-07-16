#ifndef USERMAINSCN_H
#define USERMAINSCN_H

#include <QWidget>
#include <qmap.h>
#include"QTimer"
#include"qjsonobject.h"
#include"qjsonarray.h"
#include"qjsondocument.h"
#include"qsqltablemodel.h"
#include"userdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UserMainScn; }
QT_END_NAMESPACE

class UserMainScn : public QWidget
{
    Q_OBJECT

public:
    UserMainScn(UserData user,QWidget *parent = nullptr);
    ~UserMainScn();

private slots:
    void onReceiveMsg(QByteArray data);

    void onSearchExamTimeout();

    void onFloatLabelTimeout();

    void on_btnStartExam_clicked();

    void on_btnClose_clicked();

    void on_btnMini_clicked();

    void on_pushButton_5_clicked();

    void on_btnStartExercise_clicked();

    void on_btnSelectModel_clicked();

    void on_btnShowExamInfo_clicked();

    void on_btnErrorReset_clicked();

    void on_btnCloseSys_clicked();

    void on_btnSetUserData_clicked();

private:
    void initSelectModel();

private:
    QSqlTableModel *selectModel;
    QString currClassid;
    QJsonArray examArray;
    int currHasExam;
    QMap<QString,QDateTime>examStartTimeMap;
    QMap<QString,QDateTime>examStopTimeMap;
    int floatLabelAx;
    QTimer *floatLabelTimer;
    QTimer *searchExamTimer;
    Ui::UserMainScn *ui;
};
#endif // USERMAINSCN_H
