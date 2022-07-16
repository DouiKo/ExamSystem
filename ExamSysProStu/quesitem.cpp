#include "quesitem.h"
#include "ui_quesitem.h"
#include"QDebug"

QuesItem::QuesItem(int quesType, QString ques, int number, bool isFree, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuesItem)
{
    ui->setupUi(this);
    this->quesType = quesType;
    this->quesStr = ques;
    this->quesId = number;
    selectId = "";
    currIsFree = isFree;
    title = ques;
    ui->plainTextEdit->setPlainText(ques);
}

QuesItem::~QuesItem()
{
    delete ui;
}

void QuesItem::addNewChoice(QString choice)
{
    int num = choiceMap.keys().size();
    choiceMap[num] = choice;
    if(quesType == 0){
        QRadioButton *radioButton = new QRadioButton(choice);
        radioButton->setStyleSheet("font: 11pt '黑体'");
        radioButton->setMinimumHeight(30);
        ui->verticalLayout->addWidget(radioButton);
        connect(radioButton,&QRadioButton::clicked,this,&QuesItem::choiceButtonClicked);
    }
    if(quesType == 1){
        QCheckBox *checkBox = new QCheckBox(choice);
        checkBox->setStyleSheet("font: 11pt '黑体'");
        checkBox->setMinimumHeight(30);
        ui->verticalLayout->addWidget(checkBox);
        connect(checkBox,&QCheckBox::toggled,this,&QuesItem::choiceCheckBoxButtonClicked);
    }
    if(quesType == 2){
        QRadioButton *radioButton = new QRadioButton(choice);
        radioButton->setStyleSheet("font: 11pt '黑体'");
        radioButton->setMinimumHeight(30);
        ui->verticalLayout->addWidget(radioButton);
        connect(radioButton,&QRadioButton::clicked,this,&QuesItem::choiceButtonClicked);
    }
}

void QuesItem::setTrueResult(QString resultId)
{
    trueResult = resultId;
}

QString QuesItem::getTrueResult()
{
    return trueResult;
}

QString QuesItem::getSelectItemId()
{
    return selectId;
}

QString QuesItem::getTitle()
{
    return title;
}

QString QuesItem::getChoice(int choice)
{
    return choiceMap[choice];
}

int QuesItem::getItemId()
{
    return quesId;
}

int QuesItem::getQuesType()
{
    return quesType;
}

int QuesItem::getScore()
{
    return score;
}

void QuesItem::setScore(int score)
{
    this->score = score;
}

void QuesItem::choiceCheckBoxButtonClicked(bool checked)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox *>(sender());
    if(checked){
        selectId.append(checkBox->text()+" ");
    }else{
        selectId.remove(checkBox->text()+" ");
    }
    emit quesStateChanged();
    onCurrIsFree();
}

void QuesItem::choiceButtonClicked()
{
    QRadioButton *radioButton = qobject_cast<QRadioButton *>(sender());
    selectId = radioButton->text();
    emit quesStateChanged();
    onCurrIsFree();
}

void QuesItem::onCurrIsFree()
{
//    if(currIsFree){
//        ui->textAnswer->setText(replaceNumberToUpper(getTrueResult()));
//        ui->textAns->show();
//        ui->textAnswer->show();
//    }
}
