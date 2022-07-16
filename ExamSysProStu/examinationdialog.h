#ifndef EXAMINATIONDIALOG_H
#define EXAMINATIONDIALOG_H

#include <QDialog>
#include <qdatetime.h>
#include"qjsonarray.h"
#include"quesitem.h"
#include"quesbutton.h"

namespace Ui {
class ExaminationDialog;
}

class ExaminationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExaminationDialog(QDateTime startTime,QDateTime stopTime,QString username, QString classid, QJsonArray jsonArray,QString examid,int type = 0, QWidget *parent = nullptr);
    ~ExaminationDialog();

private slots:
    void onQuesButtonClicked();
    void onQuesStateChanged();

    void on_btnClose_clicked();

private:
    void billExam();
    void timerEvent(QTimerEvent *e)override;
    void closeEvent(QCloseEvent *e) override;
    void createErrorJson(QuesItem *item);
    QJsonArray quesArray;
    QJsonArray errorArray;
    int examType;
    int flag;
    QString examid;
    QDateTime startDateTime;
    QDateTime stopDateTime;
    QMap<QuesItem *,QuesButton *>quesMap;
    QList<QuesItem *>quesList;
    Ui::ExaminationDialog *ui;
};

#endif // EXAMINATIONDIALOG_H
