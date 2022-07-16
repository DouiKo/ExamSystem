#include "chartdialog.h"
#include "ui_chartdialog.h"

ChartDialog::ChartDialog(double single, double much, double tf, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartDialog)
{
    ui->setupUi(this);
    setWindowTitle("数据统计信息");
    rightSingle = single;
    rightMuch = much;
    righttf = tf;
    createChartView();
}

ChartDialog::~ChartDialog()
{
    delete ui;
}

void ChartDialog::onSliceHovered(bool check)
{
    QPieSlice *slice = qobject_cast<QPieSlice *>(sender());
    slice->setExplodeDistanceFactor(0.2);
    slice->setExploded(check);
}

void ChartDialog::createChartView()
{
    QChartView *chartView;
    chartView = new QChartView(createPieCharts());
    chartView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->gridLayout->addWidget(chartView, 0, 0);
}

QChart *ChartDialog::createPieCharts()
{
    QChart *chart = new QChart();
    chart->setTitle("考试信息统计");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QPieSeries *series = new QPieSeries(chart);
    series->setHoleSize(0);
    ///[设置数据]
    series->append("单选得分率",rightSingle);
    series->append("多选得分率",rightMuch);
    series->append("判断得分率",righttf);
    ///[设置数据]
    QPieSlice *slice = nullptr;
    for (int i = 0; i < 2; i++) {
        slice = series->slices().at(i);
        slice->setLabel(slice->label()+QString::asprintf(" :  %.1f%%",slice->percentage()*100));
        connect(slice,&QPieSlice::hovered,this,&ChartDialog::onSliceHovered);
        slice->setExplodeDistanceFactor(0.05);
        slice->setLabelVisible();
    }
    slice->setExploded();
    series->setPieSize(0.8);
    chart->addSeries(series);
    return chart;
}
