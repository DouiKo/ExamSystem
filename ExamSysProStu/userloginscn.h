#ifndef USERLOGINSCN_H
#define USERLOGINSCN_H

#include <QWidget>
#include"extcpsocket.h"
#include"usermainscn.h"

namespace Ui {
class UserLoginScn;
}

class UserLoginScn : public QWidget
{
    Q_OBJECT

public:
    explicit UserLoginScn(QWidget *parent = nullptr);
    ~UserLoginScn();

private slots:

    void receiveMsg(QByteArray data);

    void on_btnLogin_clicked();

    void on_btnReg_clicked();

    void on_btnMini_clicked();

    void on_btnClose_clicked();

    void on_btnBackup_clicked();

private:
    UserMainScn *mainScn;

private:
    QPoint oldPos;
    bool press;
    Ui::UserLoginScn *ui;
    void setRoundWidget();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // USERLOGINSCN_H
