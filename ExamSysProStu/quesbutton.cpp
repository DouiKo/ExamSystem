#include "quesbutton.h"
#include"QDebug"

QuesButton::QuesButton(QWidget *parent) :
    QPushButton(parent)
{
    connect(this,&QuesButton::doneStateChanged,this,&QuesButton::onDoneStateChanged);
    isDone = false;
    this->setStyleSheet("QPushButton{font: 11pt '黑体';}");
}

void QuesButton::setDoneState(bool state)
{
    isDone = state;
    emit doneStateChanged(state);
}

bool QuesButton::getDoneState()
{
    return isDone;
}

void QuesButton::onDoneStateChanged(bool state)
{
    if (state) {
        this->setStyleSheet("QPushButton{font: 11pt '黑体';border:none;background-color: rgb(0, 126, 0);}");
    } else {
        this->setStyleSheet("QPushButton{font: 11pt '黑体';}");
    }
}
