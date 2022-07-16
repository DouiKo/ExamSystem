#ifndef QUESITEM_H
#define QUESITEM_H

#include <QWidget>
#include"QMap"
#include"QRadioButton"
#include"QCheckBox"

namespace Ui {
class QuesItem;
}

class QuesItem : public QWidget
{
    Q_OBJECT

public:
    explicit QuesItem(int quesType,QString ques,int number,bool isFree = false,QWidget *parent = nullptr);
    ~QuesItem();
    void addNewChoice(QString choice);
    void setTrueResult(QString resultId);
    QString getTrueResult();
    QString getSelectItemId();
    QString getTitle();
    QString getChoice(int choice);
    int getItemId();
    int getQuesType();
    int getScore();
    void setScore(int score);
private slots:
    void choiceCheckBoxButtonClicked(bool checked);

    void choiceButtonClicked();

private:
    void onCurrIsFree();
    bool currIsFree;
    QString title;
    QString trueResult;
    int quesId;
    int score;
    QString quesStr;
    int quesType;
    QMap<int,QString>choiceMap;
    QString selectId;
    Ui::QuesItem *ui;

signals:
    void goToHome();
    void pageChanged(int);
    void quesStateChanged();
};

#endif // QUESITEM_H
