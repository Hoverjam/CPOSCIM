#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

#include <QWinTaskbarProgress>
#include <QWinTaskbarButton>

#include "powertablemodel.h"
#include "traintablemodel.h"
#include "queryresultthread.h"
#include "saveresultthread.h"
#include "dialogwithonebutn.h"
#include "dialogwithtwobutn.h"
#include "calculate.h"
#include "stopthread.h"
#include "morechartswidget.h"

QT_CHARTS_USE_NAMESPACE
#include "chartview.h"
#include "mousechart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_newPowerRow_clicked();
    void on_delPowerRow_clicked();
    void on_newTrainRow_clicked();
    void on_delTrainRow_clicked();

    void onTableClicked0(const QModelIndex &);
    void onTableClicked1(const QModelIndex &);

    void IChartHovered(const QPointF &point, bool state);
    void chartHovered(const QPointF &point, bool state);

    void on_queryData_clicked();
    void on_exportData_clicked();

    void dataChange4I(QPointF);
    void currentDataChange(QPointF);

    void powerNumChanged(int num);
    void trainNumChanged(int num);
    void saveDataDone(QString status);

    void on_insertPowerRow_clicked();
    void on_insertTrainRow_clicked();
    void on_delAllPowerRow_clicked();
    void on_delAllTrainRow_clicked();
    void deletAllRows(int who);

    void on_action_S_triggered();
    void on_action_L_triggered();
    void on_action_K_triggered();
    void on_action_G_triggered();
    void on_action_F_triggered();
    void on_action_H_triggered();
    void on_action_A_triggered();

    void on_startCal_clicked();
    void onCalDone(double *, int, double, double, double, bool);
    void IDone(double *, int, double, double, double);
    void I0Done();
    void I1Done(double *, double *, int, double, double, double, double, double);
    void I2Done(double *, double *, double *, int, double, double, double, double, double, double, double);
    void onCalFail(int);

    void dialogWithOneButtonAttentionSet();

    void on_pauseCal_clicked();

    void on_continueCal_clicked();

    void on_stopCal_clicked();
    void onCalStopped();
    void calStatus(int);

    void on_action_E_triggered();
    void on_action_T_triggered();
    void on_action_F_2_triggered();

    void on_exportIData_clicked();
    void on_action_I_triggered();

    void on_showIWidget_clicked();

    void onpPosReturn(const QString &text);
    void onpVolReturn(const QString &text);
    void onpResReturn(const QString &text);
    void ontPosReturn(const QString &text);
    void ontCurReturn(const QString &text);

    void onPDataChange4G(int row, int column, QString str);
    void onTDataChange4G(int row, int column, QString str);

    void on_close0_clicked();

    void on_close1_clicked();

    void on_close2_clicked();

    void on_action_S_2_triggered();

    void on_showModel_clicked();

private:
    Ui::MainWindow *ui;

    QStatusBar *statusbar2;
    QStatusBar *statusbar3;

    PowerTableModel *powerModel;
    TrainTableModel *trainModel;

    QVector<qreal> test1;
    QVector<qreal> test2;
    QVector<qreal> test3;
    QVector<qreal> testFor3;

    QueryResultThread queThread4Chart;
    QueryResultThread queThread4IChart;

    QueryResultThread *queThread;
    QueryResultThread *queThread4I;
    SaveResultThread *savThread;

    QLabel *powerNum;
    QLabel *trainNum;
    QLabel *igModel;
    QLabel *statusHint;

    DialogWithOneButn *dialogWithOneButn;
    DialogWithTwoButn *dialogWithTwoButn;

    QGridLayout *gridLayout;
    chartView *chartView4I;
    chartView *m_ChartView;

    MouseChart *chart4I;// 图表对象
    QValueAxis *axisX4I; // x坐标轴
    QValueAxis *axisY4I; // y坐标轴
    QLineSeries *lineSeriesI; // 系列I
    QPen penSeries4I;  // 画笔

    MouseChart *chart4V;// 图表对象
    QValueAxis *m_axisX; // x坐标轴
    QValueAxis *m_axisY; // y坐标轴
    QLineSeries *m_lineSeries1; // 系列1
    QPen m_penSeries1;  // 系列1的画笔

    calculate *startcal;
    StopThread *sThread;
    QProgressBar *pBar;

    int igFlag;

    QModelIndex indexP;
    QModelIndex indexT;
    int whichP = -1;
    int whichT = -1;

    QWinTaskbarButton *windowsTaskbarButton;
    QWinTaskbarProgress *windowsTaskbarProgress;

    MoreChartsWidget *widgetI0;
    MoreChartsWidget *widgetI1;
    MoreChartsWidget *widgetI2;

    int moreChartsFlag = -1;

    qreal getData_1(qreal x);
    qreal getData_2(qreal x);

    QVector<double> powerPos;
    QVector<double> trainPos;

    void setChartI();
    void setChartV();
};
#endif // MAINWINDOW_H
