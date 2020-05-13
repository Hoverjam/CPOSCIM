#include "morechartswidget.h"
#include "ui_morechartswidget.h"

MoreChartsWidget::MoreChartsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoreChartsWidget)
    , queThread(new QueryResultThread(this))

{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);


    ui->gridLayout->setMargin(0);

    chartView4I = new chartView(ui->gridLayout->widget());
    chartView4I->setRenderHint(QPainter::Antialiasing, true);
    ui->gridLayout->addWidget(chartView4I, 0, 0);
    setChart();
    chart4I->layout()->setContentsMargins(10, 10, 10, 10);
}

MoreChartsWidget::~MoreChartsWidget()
{
    delete ui;
}
void MoreChartsWidget::setChart()
{
    chart4I = new MouseChart();// 图表对象

    lineSeriesI = new QLineSeries(this); // 系列I
    penSeries4I = QPen(QColor(0,120,215), 5);  // 画笔
    lineSeriesI->setPen(penSeries4I);
    // 为折线添加初始数据
    qreal x = 0.f;
    for (int i = 0; i <= c_MaxSize; i++) {
        x = i * 1.f / c_MaxSize;
        test2<<getData_1(x);
        lineSeriesI->append(i, test2[i]);
    }
    queThread->data0(test1,test2,-10,0);
    queThread4Chart.data0(test1,test2,-10,0);

    // 系列添加到图表
    chart4I->addSeries(lineSeriesI);

    // 隐藏图例
    chart4I->legend()->hide();

    // 标题
    chart4I->setTitle("title");
    QFont fontTitle;
    fontTitle.setFamily(tr("微软雅黑"));
    fontTitle.setPointSize(12);
    chart4I->setTitleFont(fontTitle);

    axisX4I = new QValueAxis(); // x坐标轴
    axisX4I->setRange(0, 100);
    axisX4I->setTitleText("距离(km)"); // 标题
    axisX4I->setTickCount(10);   // 刻度数

    axisY4I = new QValueAxis();
    axisY4I->setRange(-10, 10);
    axisY4I->setTitleText("杂散电流(A)");  // 标题
    axisY4I->setTickCount(10);

    // 刻度
    QFont fontAxis;
    fontAxis.setFamily(tr("微软雅黑"));
    fontAxis.setPointSize(10);
    axisX4I->setTitleFont(fontAxis);
    axisY4I->setTitleFont(fontAxis);
    // 坐标轴字体
    QFont fontLabel;
    fontLabel.setFamily(tr("微软雅黑"));
    fontLabel.setPointSize(9);
    axisX4I->setLabelsFont(fontLabel);
    axisY4I->setLabelsFont(fontLabel);

    // 坐标轴与系列绑定
    chart4I->setAxisX(axisX4I, lineSeriesI);
    chart4I->setAxisY(axisY4I, lineSeriesI);
    //设置tip范围
    chart4I->setTipRange(0,100,-10,10);

    // 开启动画
    chart4I->setAnimationOptions(QChart::SeriesAnimations);
    // 绑定鼠标悬浮信号
    connect(chart4I,&MouseChart::hover,this, &MoreChartsWidget::chartHovered);
    connect(&queThread4Chart, &QueryResultThread::done, chartView4I, &chartView::slot_showTip);

    chartView4I->setChart(chart4I);
}

void MoreChartsWidget::chartHovered(const QPointF &point, bool state)
{
    if(state == true)
    {
        queThread4Chart.data(point.x()/c_MaxSize,point.x());
        queThread4Chart.justRun();
    }
    else
    {
        chartView4I->slot_showTip(point,0,0,false,0);
    }
}

void MoreChartsWidget::data(double *I, int nL_1, double lOfDev, double IMin, double IMax, int flag, QVector<double> powerPos, QVector<double> trainPos)
{
    QDesktopWidget *deskdop=QApplication::desktop();

    if(flag==0)
    {
        chart4I->setTitle("距离-排流网杂散电流图");
        move((deskdop->width()-this->width())/2-40, (deskdop->height()-this->height())/2-40);
    }
    else if(flag==1)
    {
        chart4I->setTitle("距离-埋地金属杂散电流图");
        move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);
    }
    else if(flag==2)
    {
        chart4I->setTitle("距离-大地杂散电流图");
        move((deskdop->width()-this->width())/2+40, (deskdop->height()-this->height())/2+40);
    }

    test1.clear();
    test2.clear();
    lineSeriesI->clear();
    //第一个点
    test1<<0;
    test2<<qAbs(*(I)-(*(I+1)-*(I))/2);
    lineSeriesI->append(test1[0], test2[0]);
    //中间点
    for (int i = 0; i < nL_1; i++) {
        test1<<(i*lOfDev+lOfDev/2);
        test2<<*(I+i);
        lineSeriesI->append(test1[i+1], test2[i+1]);
    }
    //最后一个点
    test1<<nL_1*lOfDev;
    test2<<qAbs(I[nL_1-1]+(I[nL_1-1]-I[nL_1-2])/2);
    lineSeriesI->append(test1[nL_1+1], test2[nL_1+1]);

    axisX4I->setRange(test1[0], test1[nL_1+1]);
    axisY4I->setRange(IMin-(IMax-IMin)*0.1, IMax+(IMax-IMin)*0.1);

    queThread->data0(test1,test2,IMin-(IMax-IMin)*0.1,0);
    queThread4Chart.data0(test1,test2,IMin-(IMax-IMin)*0.1,0);
    chart4I->setTipRange(test1[0], test1[nL_1+1], IMin-(IMax-IMin)*0.1, IMax+(IMax-IMin)*0.1);
    c_MaxSize = test1[nL_1+1];

    chartView4I->showPT(powerPos, trainPos, IMin-(IMax-IMin)*0.1);
}

qreal MoreChartsWidget::getData_1(qreal /*x*/) {  //测试用数据
    return 0;
}
qreal MoreChartsWidget::getData_2(qreal /*x*/) {
    return 0;
}
