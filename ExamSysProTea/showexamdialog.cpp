#include "showexamdialog.h"
#include "ui_showexamdialog.h"
#include"qdebug.h"
#include <QJsonDocument>
#include <qjsonobject.h>

ShowExamDialog::ShowExamDialog(QJsonArray jsonArray, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowExamDialog)
{
    ui->setupUi(this);
    setWindowTitle("查看试卷");
    QJsonDocument jsonDocument(jsonArray);
    QString value = jsonDocument.toJson();
    value = value.remove("[");
    value = value.remove("]");
    ui->textEdit->setText(value);
}

ShowExamDialog::~ShowExamDialog()
{
    delete ui;
}

void ShowExamDialog::on_pushButton_clicked()
{
    this->close();
}
