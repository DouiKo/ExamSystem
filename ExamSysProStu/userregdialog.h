#ifndef USERREGDIALOG_H
#define USERREGDIALOG_H

#include <QDialog>

namespace Ui {
class UserRegDialog;
}

class UserRegDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserRegDialog(QWidget *parent = nullptr);
    ~UserRegDialog();

private slots:
    void receiveMsg(QByteArray data);

    void on_btnReg_clicked();

private:
    Ui::UserRegDialog *ui;
};

#endif // USERREGDIALOG_H
