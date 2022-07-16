#ifndef USERBACKUPDIALOG_H
#define USERBACKUPDIALOG_H

#include <QDialog>

namespace Ui {
class UserBackupDialog;
}

class UserBackupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserBackupDialog(QWidget *parent = nullptr);
    ~UserBackupDialog();

private slots:
    void on_btnReturnPassword_clicked();

    void receiveMsg(QByteArray data);
private:
    Ui::UserBackupDialog *ui;
};

#endif // USERBACKUPDIALOG_H
