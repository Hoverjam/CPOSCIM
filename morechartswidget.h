#ifndef MORECHARTSWIDGET_H
#define MORECHARTSWIDGET_H

#include <QWidget>
#include <QtCharts>
#include "queryresultthread.h"
#include "saveresultthread.h"

QT_CHARTS_USE_NAMESPACE
#include "chartview.h"
#include "mousechart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MoreChartsWidget; }
QT_END_NAMESPACE

namespace Ui {
class MoreChartsWidget;
}

class MoreChartsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MoreChartsWidget(QWidget *parent = nullptr);
    ~MoreChartsWidget();

   void data(double *, int, double, double, double, int, QVector<double>, QVector<double>);

protected:
    void setChart();

private slots:
    void chartHovered(const QPointF &point, bool state);

private:
    Ui::MoreChartsWidget *ui;

    QVector<qreal> test1;
    QVector<qreal> test2;

    QueryResultThread queThread4Chart;
    QueryResultThread *queThread;

    chartView *chartView4I;
    MouseChart *chart4I;// 图表对象
    QValueAxis *axisX4I; // x坐标轴
    QValueAxis *axisY4I; // y坐标轴
    QLineSeries *lineSeriesI; // 系列I
    QPen penSeries4I;  // 画笔

    qreal c_MaxSize = 100; //测试用数据长度

    qreal getData_1(qreal x);
    qreal getData_2(qreal x);
};

#endif // MORECHARTSWIDGET_H
