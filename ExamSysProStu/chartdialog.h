#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include"QChart"
#include"QChartView"
#include"QPieSeries"

namespace Ui {
class ChartDialog;
}

using namespace QtCharts;

class ChartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChartDialog(double single,double much,double tf,QWidget *parent = nullptr);
    ~ChartDialog();

private slots:
    void onSliceHovered(bool check);

private:
    void createChartView();
    QChart *createPieCharts();
private:
    double righttf;
    double rightSingle;
    double rightMuch;
    Ui::ChartDialog *ui;
};

#endif // CHARTDIALOG_H
