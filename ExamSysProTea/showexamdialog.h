#ifndef SHOWEXAMDIALOG_H
#define SHOWEXAMDIALOG_H

#include <QDialog>
#include <qjsonarray.h>

namespace Ui {
class ShowExamDialog;
}

class ShowExamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowExamDialog(QJsonArray jsonArray,QWidget *parent = nullptr);
    ~ShowExamDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ShowExamDialog *ui;
};

#endif // SHOWEXAMDIALOG_H
