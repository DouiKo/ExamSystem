#ifndef USERMAINSCN_H
#define USERMAINSCN_H

#include <QWidget>
#include"qsqltablemodel.h"
#include"qsqltablemodel.h"
#include"qcheckbox.h"
#include"qjsonarray.h"
#include"showexamdialog.h"
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

private slots:   //界面跳转
    void receiveMsg(QByteArray data);

    void on_btnClose_clicked();

    void on_btnMini_clicked();

    void on_btnAddSingle_clicked();

    void on_btnAddMuch_clicked();

    void on_btnAddTof_clicked();

    void on_btnTofReturn_clicked();

    void on_btnSingleReturn_clicked();

    void on_btnMuchReturn_clicked();

    void on_btnShowQues_clicked();

private slots:  //添加操作
    void on_btnAddQuesSingle_clicked();

    void on_btnAddQuesMuch_clicked();

    void on_btnAddQuesToF_clicked();

    void on_btnExcelImport_clicked();

private slots:
    void on_btnSelectModel_clicked();

    void on_btnDeleteSelectModel_clicked();

    void on_btnGoBack_clicked();

    void on_btnQuesExportExcel_clicked();

    void on_btnQuesNone_clicked();

    void on_btnSelectQuesModel_clicked();

    void on_btnAddExam_clicked();

    void on_btnQuesCommit_clicked();

    void on_btnExamReturn_clicked();

    void on_btnAddExamCommit_clicked();

    void on_btnPreviewReturn_clicked();

    void on_singleSpinBox_valueChanged(int arg1);

    void on_muchSpinBox_valueChanged(int arg1);

    void on_tofSpinBox_valueChanged(int arg1);

    void on_btnAddRandExam_clicked();

    void on_btnExamRandReturn_clicked();

    void on_btnRandExam_clicked();

    void on_btnExcelImportExam_clicked();

    void on_btnGoBackExamSelect_clicked();

    void on_btnSelectExam_clicked();

    void on_btnDeleteSelectExam_clicked();

    void on_btnShowExam_clicked();

    void on_btnShowExamInfo_clicked();

    void on_btnStartExam_clicked();

    void on_btnSelectExamLogModel_clicked();

    void on_btnErrorReset_clicked();

    void on_btnExamLogExportExcel_clicked();

    void on_btnSetUserData_clicked();

    void on_btnCloseSys_clicked();

private:
    void initExamLogModel();
    void initSelectModel();
    void initQuesModel();
    void initSelectExamListModel();
    void createExamListJson(QString title,QString type,QString c1,QString c2,QString c3,QString c4,QString c5,QString c6,QString result,QString score);
    void createExamInformationJson(int singleCount,int muchCount,int tofCount,int singleScore,int muchScore,int tofScore);
    QJsonObject addChoiceQuesJson(QString title,QString type,QString c1 = "",QString c2 = "",QString c3 = "",QString c4 = "",QString c5 = "",QString c6 = "",QString result = "",QString level = "",QString score = "");
    void initRandQuesArray();
private:
    QSqlTableModel *examlogModel;
    QJsonArray quesJsonArray;
    QList<QCheckBox *>checkBoxList;
    QSqlTableModel *selectModel;
    QSqlTableModel *quesModel;
    QSqlTableModel *examModel;
    Ui::UserMainScn *ui;
};
#endif // USERMAINSCN_H
