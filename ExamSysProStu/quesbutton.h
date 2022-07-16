#ifndef QUESBUTTON_H
#define QUESBUTTON_H

#include <QObject>
#include"QPushButton"

class QuesButton : public QPushButton
{
    Q_OBJECT
public:
    QuesButton(QWidget *parent = nullptr);
    void setDoneState(bool state);
    bool getDoneState();
private:
    bool isDone;
private slots:
    void onDoneStateChanged(bool state);
signals:
    void doneStateChanged(bool);
};

#endif // QUESBUTTON_H
