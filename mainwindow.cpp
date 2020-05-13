#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lineeditqrealdelegate.h"
#include <QColor>
#include <QDockWidget>
#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QColorDialog>

qreal c_MaxSize = 300; //测试用数据长度

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , queThread(new QueryResultThread(this))
    , queThread4I(new QueryResultThread(this))
    , savThread(new SaveResultThread(this))
    , dialogWithOneButn(new DialogWithOneButn(this))
    , dialogWithTwoButn(new DialogWithTwoButn(this))
    , startcal(new calculate(this))
    , sThread(new StopThread(this))
    , pBar(new QProgressBar(this))
    , windowsTaskbarButton(new QWinTaskbarButton(this))
    , widgetI0(new MoreChartsWidget(this))
    , widgetI1(new MoreChartsWidget(this))
    , widgetI2(new MoreChartsWidget(this))

{
    ui->setupUi(this);
    statusbar2 = new QStatusBar(this);
    statusbar3 = new QStatusBar(this);

    ui->horizontalLayout4status2->addWidget(statusbar2);
    ui->horizontalLayout4status3->addWidget(statusbar3);
    statusbar2->setSizeGripEnabled(false);
    statusbar3->setSizeGripEnabled(false);

    QPalette pal = ui->widget0->palette();
    QPixmap pixmap0(":/images/icos/2ceng_458x183.png");
    pal.setBrush(QPalette::Background, QBrush(pixmap0));
    ui->widget0->setPalette(pal);

//chart（开始）
    gridLayout = new QGridLayout(ui->chartWidget);
    ui->chartWidget->setLayout(gridLayout);
    gridLayout->setMargin(0);

    chartView4I = new chartView(ui->chartWidget);
    chartView4I->setRenderHint(QPainter::Antialiasing, true);
    gridLayout->addWidget(chartView4I, 0, 0);
    setChartI();
    chart4I->layout()->setContentsMargins(3, 3, 3, 3);
    m_ChartView = new chartView(ui->chartWidget);
    m_ChartView->setRenderHint(QPainter::Antialiasing, true);
    gridLayout->addWidget(m_ChartView, 1, 0);
    setChartV();
    chart4V->layout()->setContentsMargins(3, 4, 3, 2);
//chart（结束）

//查询与导出（开始）
    //LineEdit设置输入限制
    QDoubleValidator *validator = new QDoubleValidator(0,99999999,20,this);
    ui->distance->setValidator(validator);
    ui->current->setValidator(validator);
    ui->railRes->setValidator(validator);
    ui->earthRes->setValidator(validator);
    ui->transitionRes->setValidator(validator);
    ui->catenaryRes->setValidator(validator);
    ui->lenthOfRail->setValidator(validator);
    ui->lenthOfDev->setValidator(validator);
    ui->current0->setValidator(validator);
    ui->drainage->setValidator(validator);
    ui->metal->setValidator(validator);
    ui->trans2->setValidator(validator);
    ui->trans3->setValidator(validator);
    ui->pPos->setValidator(validator);
    ui->pVol->setValidator(validator);
    ui->pRes->setValidator(validator);
    ui->tPos->setValidator(validator);
    ui->r2eP->setValidator(validator);
    QDoubleValidator *validator1 = new QDoubleValidator(this);
    ui->tCur->setValidator(validator1);

//    ui->railRes->setText("0.02");
//    ui->earthRes->setText("0.01");
//    ui->transitionRes->setText("0.0666667");
//    ui->catenaryRes->setText("0.02");
//    ui->lenthOfRail->setText("10");
//    ui->lenthOfDev->setText("0.05");

    connect(ui->distance, &QLineEdit::returnPressed, this, &MainWindow::on_queryData_clicked);//距离输入框响应回车
    connect(queThread4I, &QueryResultThread::done, this, &MainWindow::dataChange4I);
    connect(queThread, &QueryResultThread::done, this, &MainWindow::currentDataChange);//查询线程结束修改杂散电流框数据
    connect(this,&MainWindow::destroyed,queThread, &QueryResultThread::terminate);
    connect(this,&MainWindow::destroyed,queThread4I, &QueryResultThread::terminate);
    connect(savThread,&SaveResultThread::saved,this, &MainWindow::saveDataDone);//导出完成提示
    connect(this,&MainWindow::destroyed,savThread,&SaveResultThread::terminate);
//查询与导出（结束）

//tableview（开始）
    powerModel = new PowerTableModel(this);
    ui->powerTable->setModel(powerModel);
    ui->powerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  //表右侧匹配控件边缘
    trainModel = new TrainTableModel(this);
    ui->trainTable->setModel(trainModel);
    ui->trainTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  //表右侧匹配控件边缘
    //tableview设置输入限制代理
    LineEditQrealDelegate *delegate = new LineEditQrealDelegate(this);
    ui->powerTable->setItemDelegateForColumn(1,delegate);
    ui->powerTable->setItemDelegateForColumn(2,delegate);
    ui->powerTable->setItemDelegateForColumn(3,delegate);
    ui->trainTable->setItemDelegateForColumn(1,delegate);
    LineEditQrealDelegate *delegate1 = new LineEditQrealDelegate(this,1);
    ui->trainTable->setItemDelegateForColumn(2,delegate1);
    //删除所有table行的信号与槽
    connect(dialogWithTwoButn,&DialogWithTwoButn::tru,this,&MainWindow::deletAllRows);

    connect(ui->powerTable, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked0(const QModelIndex &)));
    connect(ui->trainTable, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked1(const QModelIndex &)));
    connect(powerModel,&PowerTableModel::dataChange4G, this, &MainWindow::onPDataChange4G);
    connect(trainModel,&TrainTableModel::dataChange4G, this, &MainWindow::onTDataChange4G);

//tableview（结束）

//一堆需要响应回车的输入框------------------------------------------------------------------------------------------------------
    connect(ui->lenthOfRail, &QLineEdit::returnPressed, this, &MainWindow::on_startCal_clicked);
    connect(ui->lenthOfDev, &QLineEdit::returnPressed, this, &MainWindow::on_startCal_clicked);
    connect(ui->pPos, &QLineEdit::textChanged, this, &MainWindow::onpPosReturn);
    connect(ui->pVol, &QLineEdit::textChanged, this, &MainWindow::onpVolReturn);
    connect(ui->pRes, &QLineEdit::textChanged, this, &MainWindow::onpResReturn);
    connect(ui->tPos, &QLineEdit::textChanged, this, &MainWindow::ontPosReturn);
    connect(ui->tCur, &QLineEdit::textChanged, this, &MainWindow::ontCurReturn);
//--------------------------------------------------------------------------------------------------------------------------
//计算状态按钮显示
    ui->continueCal->hide();
    ui->pauseCal->hide();
    ui->stopCal->hide();
//计算结果显示槽
    connect(startcal,&calculate::done,this,&MainWindow::onCalDone);
    connect(startcal,&calculate::doneI,this,&MainWindow::IDone);
    connect(startcal,&calculate::doneI0,this,&MainWindow::I0Done);
    connect(startcal,&calculate::doneI1,this,&MainWindow::I1Done);
    connect(startcal,&calculate::doneI2,this,&MainWindow::I2Done);
    connect(startcal,&calculate::notDone,this,&MainWindow::onCalFail);
    connect(this,&MainWindow::destroyed,startcal,&calculate::terminate);
    connect(sThread,&StopThread::calStopped,this,&MainWindow::onCalStopped);
    connect(this,&MainWindow::destroyed,sThread,&StopThread::terminate);
    connect(startcal,&calculate::status,this,&MainWindow::calStatus);

//状态栏初始化
    powerNum = new QLabel(this);
    powerNum->setText("牵引站数量："+QString::number(powerModel->pSize())+"         ");
    statusbar3->insertPermanentWidget(0,powerNum);
    trainNum = new QLabel(this);
    trainNum->setText("负荷数量："+QString::number(trainModel->tSize())+"         ");
    statusbar3->insertPermanentWidget(1,trainNum);
    igModel = new QLabel(this);
    statusbar3->insertPermanentWidget(2,igModel);
    //使高度相同
    QProgressBar *sizeB = new QProgressBar(this);
    statusbar3->insertPermanentWidget(3,sizeB);
    sizeB->hide();

    pBar->setFixedWidth(352);
    pBar->hide();
    pBar->setMinimum(0);  // 最小值
    pBar->setMaximum(100);  // 最大值
    pBar->setValue(20);  // 当前进度

    statusHint=new QLabel(this);
    statusHint->setText("Version 1.1.0 ");
    statusbar2->insertPermanentWidget(0,statusHint);
    statusbar2->insertPermanentWidget(1,pBar);   
    pBar->setStyleSheet("QProgressBar::chunk{background: #00b400;}");
    //连接数量显示信号与槽
    connect(powerModel,&PowerTableModel::rowChanged, this, &MainWindow::powerNumChanged);
    connect(trainModel,&TrainTableModel::rowChanged, this, &MainWindow::trainNumChanged);
    statusbar2->showMessage("😊 欢迎使用地铁杂散电流计算程序");

    on_action_E_triggered();//默认两层模型

    widgetI0->setWindowTitle("杂散电流1");
    widgetI1->setWindowTitle("杂散电流2");
    widgetI2->setWindowTitle("杂散电流3");

    ui->modelWidget1->hide();
    ui->modelWidget0->hide();
    ui->modelWidget2->hide();
    ui->widget4G->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//chart（开始）
qreal MainWindow::getData_1(qreal x) {  //测试用数据
    return qSin(x * 2 * M_PI) * 8;
}
//qreal MainWindow::getData_2(qreal x) {
//    return qCos(x * 2 * M_PI) * 8;
//}

void MainWindow::setChartI()
{
    chart4I = new MouseChart();// 图表对象

    lineSeriesI = new QLineSeries(this); // 系列I
    penSeries4I = QPen(QColor(0,120,215), 5);  // 画笔
    lineSeriesI->setPen(penSeries4I);
    // 为折线添加初始数据
    qreal x = 0.f;
    for (int i = 0; i <= c_MaxSize; i++) {
        test1<<i;
        x = i * 1.f / c_MaxSize;
        test3<<getData_1(x);
        lineSeriesI->append(i, test3[i]);
    }
    queThread4I->data0(test1,test3,-10,0);
    queThread4IChart.data0(test1,test3,-10,0);

    // 系列添加到图表
    chart4I->addSeries(lineSeriesI);

    // 隐藏图例
    chart4I->legend()->hide();

    // 标题
    chart4I->setTitle("距离-杂散电流图");
    QFont fontTitle;
    fontTitle.setFamily(tr("微软雅黑"));
    fontTitle.setPointSize(12);
    chart4I->setTitleFont(fontTitle);

    axisX4I = new QValueAxis(); // x坐标轴
    axisX4I->setRange(0, c_MaxSize);
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
    chart4I->setTipRange(0,c_MaxSize,-10,10);

    // 开启动画
    chart4I->setAnimationOptions(QChart::SeriesAnimations);
    // 绑定鼠标悬浮信号
    connect(chart4I,&MouseChart::hover,this, &MainWindow::IChartHovered);
    connect(&queThread4IChart, &QueryResultThread::done, chartView4I, &chartView::slot_showTip);

    chartView4I->setChart(chart4I);
}

void MainWindow::setChartV()
{
    chart4V = new MouseChart();
    // 曲线系列
    m_lineSeries1 = new QLineSeries(this);
    // 系列的画笔
    m_penSeries1 = QPen(QColor(0,120,215), 5);
    m_lineSeries1->setPen(m_penSeries1);
    // 为折线添加初始数据
    qreal x = 0.f;
    for (int i = 0; i <= c_MaxSize; i++) {
        x = i * 1.f / c_MaxSize;
        test2<<getData_1(x);
        m_lineSeries1->append(i, test2[i]);
    }
    queThread->data0(test1,test2,-10,1);
    queThread4Chart.data0(test1,test2,-10,1);

    // 系列添加到图表
    chart4V->addSeries(m_lineSeries1);

    // 隐藏图例
    chart4V->legend()->hide();

    // 标题
    chart4V->setTitle("距离-钢轨电位图");
    QFont fontTitle;
    fontTitle.setFamily(tr("微软雅黑"));
    fontTitle.setPointSize(12);
    chart4V->setTitleFont(fontTitle);

    // 构建坐标轴
    m_axisX = new QValueAxis();
    m_axisX->setRange(0, c_MaxSize);
    m_axisX->setTitleText("距离(km)"); // 标题
    m_axisX->setTickCount(10);   // 刻度数

    m_axisY = new QValueAxis();
    m_axisY->setRange(-10, 10);
    m_axisY->setTitleText("钢轨电位(V)");  // 标题
    m_axisY->setTickCount(10);

    // 刻度
    QFont fontAxis;
    fontAxis.setFamily(tr("微软雅黑"));
    fontAxis.setPointSize(10);
    m_axisX->setTitleFont(fontAxis);
    m_axisY->setTitleFont(fontAxis);
    // 坐标轴字体
    QFont fontLabel;
    fontLabel.setFamily(tr("微软雅黑"));
    fontLabel.setPointSize(9);
    m_axisX->setLabelsFont(fontLabel);
    m_axisY->setLabelsFont(fontLabel);

    // 坐标轴与系列绑定
    chart4V->setAxisX(m_axisX, m_lineSeries1);
    chart4V->setAxisY(m_axisY, m_lineSeries1);
    //设置tip范围
    chart4V->setTipRange(0,c_MaxSize,-10,10);

    // 开启动画
    chart4V->setAnimationOptions(QChart::SeriesAnimations);
    // 绑定鼠标悬浮信号
    connect(chart4V,&MouseChart::hover,this, &MainWindow::chartHovered);
    connect(&queThread4Chart, &QueryResultThread::done, m_ChartView, &chartView::slot_showTip);

    m_ChartView->setChart(chart4V);
}

void MainWindow::IChartHovered(const QPointF &point, bool state)
{
    if(state == true)
    {
        queThread4IChart.data(point.x()/c_MaxSize,point.x());
        queThread4IChart.justRun();
    }
    else
    {
        chartView4I->slot_showTip(point,0,0,false,0);
    }
}

void MainWindow::chartHovered(const QPointF &point, bool state)
{
    if(state == true)
    {
        queThread4Chart.data(point.x()/c_MaxSize,point.x());
        queThread4Chart.justRun();
    }
    else
    {
        m_ChartView->slot_showTip(point,0,0,false,1);
    }
}

//查询与导出（开始）
void MainWindow::on_queryData_clicked()
{
    statusbar2->showMessage("查询中");
    qreal queryD = ui->distance->text().toDouble();
    if(queryD>=0&&queryD<=c_MaxSize)
    {
        //开始线程
        queThread4I->data(queryD/c_MaxSize,queryD);
        queThread4I->start();
        queThread->data(queryD/c_MaxSize,queryD);
        queThread->start();
    }
    else
    {        
        dialogWithOneButn -> setMainLabel("你输入的数据可能已越界\n请检查并重新输入");
        dialogWithOneButtonAttentionSet();
        statusbar2->showMessage("😥 查询异常结束");
    }
}

void MainWindow::on_exportIData_clicked()
{
    statusbar2->showMessage("🗂 选择导出位置并为文件命名，或选择想要覆盖的文件，请确保对应文件未被其他应用（如EXCEL）占用");
    QString fileName = QFileDialog::getSaveFileName(this,"选择导出位置并为文件命名，或选择想要覆盖的文件，请确保对应文件未被其他应用（如EXCEL）占用","untitled","Excel(*.xlsx);;文本文件(*.txt)");
    if (fileName.isEmpty())
    {
        statusbar2->showMessage("🤷 导出数据已取消");
        return;
    }
    statusbar2->showMessage("文件导出中，请确保对应文件未被其他应用（如EXCEL）占用");
    //开始线程
    savThread->data(testFor3,test3,fileName,1);
    savThread->start();
}

void MainWindow::on_exportData_clicked()
{
    statusbar2->showMessage("🗂 选择导出位置并为文件命名，或选择想要覆盖的文件，请确保对应文件未被其他应用（如EXCEL）占用");
    QString fileName = QFileDialog::getSaveFileName(this,"选择导出位置并为文件命名，或选择想要覆盖的文件，请确保对应文件未被其他应用（如EXCEL）占用","untitled","Excel(*.xlsx);;文本文件(*.txt)");
    if (fileName.isEmpty())
    {
        statusbar2->showMessage("🤷 导出数据已取消");
        return;
    }
    statusbar2->showMessage("文件导出中，请确保对应文件未被其他应用（如EXCEL）占用");
    //开始线程
    savThread->data(test1,test2,fileName,0);
    savThread->start();
}

void MainWindow::dataChange4I(QPointF tempPoint)
{
    ui->current0->setText(QString::number(tempPoint.y()));
    statusbar2->showMessage("👌 查询完成");
}

void MainWindow::currentDataChange(QPointF tempPoint)
{
    ui->current->setText(QString::number(tempPoint.y()));
    statusbar2->showMessage("👌 查询完成");
}

void MainWindow::powerNumChanged(int num)
{
    powerNum->setText("牵引站数量："+QString::number(num)+"         ");
}

void MainWindow::trainNumChanged(int num)
{
    trainNum->setText("负荷数量："+QString::number(num)+"         ");
}

void MainWindow::saveDataDone(QString status)
{
    statusbar2->showMessage(status);
}

//table（开始）
void MainWindow::on_newPowerRow_clicked()
{
    powerModel->insertRows(powerModel->pSize(),1);
    whichP = powerModel->pSize()-1;
    indexP = powerModel->index(whichP, 0, QModelIndex());
    ui->label4P->setText("当前选中项为："+QString::number(whichP+1));
    ui->pPos->setText(powerModel->getSinData(1,whichP));
    ui->pVol->setText(powerModel->getSinData(2,whichP));
    ui->pRes->setText(powerModel->getSinData(3,whichP));
    ui->pVol->setFocus();
}

void MainWindow::on_insertPowerRow_clicked()
{
    if(ui->powerTable->currentIndex().row() == -1)
    {
        powerModel->insertRows(0,1);
        whichP = 0;
    }
    else
    {
        powerModel->insertRows(ui->powerTable->currentIndex().row(),1);
        whichP = ui->powerTable->currentIndex().row()-1;
    }
    indexP = powerModel->index(whichP, 0, QModelIndex());
    ui->label4P->setText("当前选中项为："+QString::number(whichP+1));
    ui->pPos->setText(powerModel->getSinData(1,whichP));
    ui->pVol->setText(powerModel->getSinData(2,whichP));
    ui->pRes->setText(powerModel->getSinData(3,whichP));
    ui->pVol->setFocus();
}

void MainWindow::on_delPowerRow_clicked()
{
    if(powerModel->pSize() == 0)
    {        
        dialogWithOneButn -> setMainLabel("全都已经删完了 🤔");
        dialogWithOneButtonAttentionSet();
        return;
    }
    if(whichP == -1)
    {
        dialogWithOneButn -> setMainLabel("你没选中任何一项 🤔");
        dialogWithOneButtonAttentionSet();
        return;
    }
    powerModel->removeRows(whichP,1);
    if(powerModel->pSize() == 0)
        {
            whichP = -1;
            ui->label4P->setText("当前选中项为：未选中任何项");
            ui->pPos->setText("");
            ui->pVol->setText("");
            ui->pRes->setText("");
        }
    else
        {
            whichP = ui->powerTable->currentIndex().row();
            indexP = powerModel->index(whichP, 0, QModelIndex());
            ui->label4P->setText("当前选中项为："+QString::number(whichP+1));
            ui->pPos->setText(powerModel->getSinData(1,whichP));
            ui->pVol->setText(powerModel->getSinData(2,whichP));
            ui->pRes->setText(powerModel->getSinData(3,whichP));
        }
}

void MainWindow::onTableClicked0(const QModelIndex &index)
{
    qDebug("%d",index.row());
    indexP = index;
    whichP = index.row();
    ui->label4P->setText("当前选中项为："+QString::number(whichP+1));
    ui->pPos->setText(powerModel->getSinData(1,whichP));
    ui->pVol->setText(powerModel->getSinData(2,whichP));
    ui->pRes->setText(powerModel->getSinData(3,whichP));

}

//------------------------------------------------------------------------------

void MainWindow::on_newTrainRow_clicked()
{
    trainModel->insertRows(trainModel->tSize(),1);
    whichT = trainModel->tSize()-1;
    indexT = trainModel->index(whichT, 0, QModelIndex());
    ui->label4T->setText("当前选中项为："+QString::number(whichT+1));
    ui->tPos->setText(trainModel->getSinData(1,whichT));
    ui->tCur->setText(trainModel->getSinData(2,whichT));
    ui->tCur->setFocus();
}

void MainWindow::on_insertTrainRow_clicked()
{
    if(ui->trainTable->currentIndex().row() == -1)
    {
        trainModel->insertRows(0,1);
        whichT = 0;
    }
    else
    {
        trainModel->insertRows(ui->trainTable->currentIndex().row(),1);
        whichT = ui->trainTable->currentIndex().row()-1;
    }
    indexT = trainModel->index(whichT, 0, QModelIndex());
    ui->label4T->setText("当前选中项为："+QString::number(whichT+1));
    ui->tPos->setText(trainModel->getSinData(1,whichT));
    ui->tCur->setText(trainModel->getSinData(2,whichT));
    ui->tCur->setFocus();
}

void MainWindow::on_delTrainRow_clicked()
{
    if(trainModel->tSize() == 0)
    {
        dialogWithOneButn -> setMainLabel("全都已经删完了 🤔");
        dialogWithOneButtonAttentionSet();
        return;
    }
    if(whichT == -1)
    {
        dialogWithOneButn -> setMainLabel("你没选中任何一项 🤔");
        dialogWithOneButtonAttentionSet();
        return;
    }
    trainModel->removeRows(whichT,1);
    if(trainModel->tSize() == 0)
        {
            whichT = -1;
            ui->label4T->setText("当前选中项为：未选中任何项");
            ui->tPos->setText("");
            ui->tCur->setText("");
        }
    else
        {
            whichT = ui->trainTable->currentIndex().row();
            indexT = trainModel->index(whichT, 0, QModelIndex());
            ui->label4T->setText("当前选中项为："+QString::number(whichT+1));
            ui->tPos->setText(trainModel->getSinData(1,whichT));
            ui->tCur->setText(trainModel->getSinData(2,whichT));
        }
}

void MainWindow::on_delAllPowerRow_clicked()
{
    if(powerModel->pSize() == 0)
    {
        dialogWithOneButn -> setMainLabel("全都已经删完了 🤔");
        dialogWithOneButtonAttentionSet();
        return;
    }
    dialogWithTwoButn -> isWho(1);
    dialogWithTwoButn -> setModal(true);
    dialogWithTwoButn -> show();
}

void MainWindow::on_delAllTrainRow_clicked()
{
    if(trainModel->tSize() == 0)
    {
        dialogWithOneButn -> setMainLabel("全都已经删完了 🤔");
        dialogWithOneButtonAttentionSet();
        return;
    }
    dialogWithTwoButn -> isWho(2);
    dialogWithTwoButn -> setModal(true);
    dialogWithTwoButn -> show();
}

void MainWindow::deletAllRows(int who)
{
    if(who == 1)
    {
        powerModel->removeRows(0,powerModel->pSize());
        whichP = -1;
        ui->label4P->setText("当前选中项为：未选中任何项");
        ui->pPos->setText("");
        ui->pVol->setText("");
        ui->pRes->setText("");
        return;
    }
    if(who == 2)
    {
        trainModel->removeRows(0,trainModel->tSize());
        whichT = -1;
        ui->label4T->setText("当前选中项为：未选中任何项");
        ui->tPos->setText("");
        ui->tCur->setText("");
    }
}

void MainWindow::onTableClicked1(const QModelIndex &index)
{
    qDebug("%d",index.row());
    indexT = index;
    whichT = index.row();
    ui->label4T->setText("当前选中项为："+QString::number(whichT+1));
    ui->tPos->setText(trainModel->getSinData(1,whichT));
    ui->tCur->setText(trainModel->getSinData(2,whichT));
}

//菜单栏开始-------------------------------------------------------------------
void MainWindow::on_action_I_triggered()
{
    on_exportIData_clicked();
}

void MainWindow::on_action_S_triggered()//导出数据
{
    on_exportData_clicked();
}

void MainWindow::on_action_L_triggered()//修改曲线颜色
{
    QColor color = QColorDialog::getColor(m_penSeries1.color(),this,tr("选择曲线颜色"));
    if(color != nullptr)
    {
        penSeries4I = QPen(color,5);
        lineSeriesI->setPen(penSeries4I);
        m_penSeries1 = QPen(color,5);
        m_lineSeries1->setPen(m_penSeries1);
    }
}

void MainWindow::on_action_K_triggered()//还原默认曲线颜色
{
    penSeries4I = QPen(QColor(0,120,215),5);
    lineSeriesI->setPen(penSeries4I);
    m_penSeries1 = QPen(QColor(0,120,215), 5);
    m_lineSeries1->setPen(m_penSeries1);
}

void MainWindow::on_action_G_triggered()//更改网格颜色
{
    QColor color = QColorDialog::getColor(m_axisX->gridLineColor(),this,tr("选择网格颜色"));
    if(color != nullptr)
    {
        axisX4I->setGridLineColor(color);
        axisY4I->setGridLineColor(color);
        m_axisX->setGridLineColor(color);
        m_axisY->setGridLineColor(color);
    }
}

void MainWindow::on_action_F_triggered()//还原默认网格颜色
{
    axisX4I->setGridLineColor(QColor(226,226,226));
    axisY4I->setGridLineColor(QColor(226,226,226));
    m_axisX->setGridLineColor(QColor(226,226,226));
    m_axisY->setGridLineColor(QColor(226,226,226));
}

void MainWindow::on_action_E_triggered()
{
    igFlag = 0;
    igModel->setText("两层模型");
    ui->labelModel->setText("两层模型");
    if(ui->modelWidget1->isVisible() || ui->modelWidget2->isVisible())
    {
        ui->modelWidget1->hide();
        ui->modelWidget2->hide();
        ui->modelWidget0->show();
    }
    ui->drainageLabel->hide();
    ui->drainage->hide();
    ui->metalLabel->hide();
    ui->metal->hide();
    ui->transLabel1->setText("钢轨-大地");
    ui->transLabel2->hide();
    ui->trans2->hide();
    ui->transLabel3->hide();
    ui->trans3->hide();
}

void MainWindow::on_action_T_triggered()
{
    igFlag = 1;
    igModel->setText("三层模型");
    ui->labelModel->setText("三层模型");
    if(ui->modelWidget0->isVisible() || ui->modelWidget2->isVisible())
    {
        ui->modelWidget0->hide();
        ui->modelWidget2->hide();
        ui->modelWidget1->show();
    }
    ui->drainageLabel->show();
    ui->drainage->show();
    ui->metalLabel->hide();
    ui->metal->hide();
    ui->transLabel1->setText("钢轨-排流网");
    ui->transLabel2->setText("排流网-大地");
    ui->transLabel2->show();
    ui->trans2->show();
    ui->transLabel3->hide();
    ui->trans3->hide();
}

void MainWindow::on_action_F_2_triggered()
{
    igFlag = 2;
    igModel->setText("四层模型");
    ui->labelModel->setText("四层模型");
    if(ui->modelWidget0->isVisible() || ui->modelWidget1->isVisible())
    {
        ui->modelWidget0->hide();
        ui->modelWidget1->hide();
        ui->modelWidget2->show();
    }
    ui->drainageLabel->show();
    ui->drainage->show();
    ui->metalLabel->show();
    ui->metal->show();
    ui->transLabel1->setText("钢轨-排流网");
    ui->transLabel2->setText("排流网-埋地金属");
    ui->transLabel3->setText("埋地金属-大地");
    ui->transLabel2->show();
    ui->trans2->show();
    ui->transLabel3->show();
    ui->trans3->show();
}

void MainWindow::on_action_H_triggered()//使用提示
{
    dialogWithOneButn -> setModal(true);
    dialogWithOneButn -> setWindowIcon(QIcon(":/images/icos/tip2.ico"));
    dialogWithOneButn -> setWindowTitle("使用提示");
    dialogWithOneButn -> setMainLabel("你可以在本软件的根目录下找到使用说明\n如果没有请前往下面网页下载");
    dialogWithOneButn -> showUrl();
    dialogWithOneButn -> show();
}

void MainWindow::on_action_A_triggered()//关于
{
    dialogWithOneButn -> setModal(true);
    dialogWithOneButn -> setWindowIcon(QIcon("qrc:/images/icos/6.ico"));
    dialogWithOneButn -> setWindowTitle("关于");
    dialogWithOneButn -> setMainLabel("Version 1.1.0 ...\n\nGUI by ZK with Qt.\nAlgorithm by HfY.");
    dialogWithOneButn -> show();
}

void MainWindow::on_startCal_clicked()
{
    //改变按钮为暂停计算和停止计算    
    statusHint->setText("开始计算 ");
    windowsTaskbarButton->setWindow(windowHandle());
    windowsTaskbarProgress = windowsTaskbarButton->progress();
    windowsTaskbarProgress->setRange(0, 100);
    windowsTaskbarProgress->setValue(0);
    windowsTaskbarProgress->show();
    pBar->setValue(0);
    pBar->show();
    ui->pauseCal->show();
    ui->stopCal->show();
    ui->startCal->hide();
    powerPos = powerModel->getPPos();
    trainPos = trainModel->getTPos();
    //向计算线程中传入数据
    startcal->setPData(powerPos, powerModel->getPVol(), powerModel->getPRes());
    startcal->setTData(trainPos, trainModel->getTCur());
    startcal->setGData(ui->catenaryRes->text().toDouble(),
                       ui->railRes->text().toDouble(),
                       ui->earthRes->text().toDouble(),
                       ui->transitionRes->text().toDouble(),
                       ui->drainage->text().toDouble(),
                       ui->trans2->text().toDouble(),
                       ui->metal->text().toDouble(),
                       ui->trans3->text().toDouble(),
                       igFlag);
    startcal->setLData(ui->lenthOfRail->text().toDouble(), ui->lenthOfDev->text().toDouble());
    startcal->setR2eData(ui->r2eP->text().toDouble(), ui->r2eCheck->isChecked());
    //开始线程
    startcal->start();
    statusHint->setText("计算中 ");
}

void MainWindow::onCalDone(double *V, int nL,double lOfDev,double vMin, double vMax, bool bl)
{
    Q_UNUSED(bl);
    test1.clear();
    test2.clear();
    m_lineSeries1->clear();
    for (int i = 0; i < nL; i++) {
        test1<<i*lOfDev;
        test2<<V[i+nL];
        m_lineSeries1->append(test1[i], test2[i]);
    }
    m_axisX->setRange(test1[0], test1[nL-1]);
    m_axisY->setRange(vMin-(vMax-vMin)*0.1, vMax+(vMax-vMin)*0.1);

    queThread->data0(test1,test2,vMin-(vMax-vMin)*0.1,1);
    queThread4Chart.data0(test1,test2,vMin-(vMax-vMin)*0.1,1);
    chart4V->setTipRange(test1[0], test1[nL-1], vMin-(vMax-vMin)*0.1, vMax+(vMax-vMin)*0.1);
    c_MaxSize = test1[nL-1];

    //给chartView传数据设置牵引供电站、负载的位置显示
    m_ChartView->showPT(powerPos, trainPos, vMin-(vMax-vMin)*0.1);

}

void MainWindow::IDone(double *I, int nL_1, double lOfDev, double IMin, double IMax)
{
    testFor3.clear();
    test3.clear();
    lineSeriesI->clear();

    //第一个点
    testFor3<<0;
    test3<<qAbs(*(I)-(*(I+1)-*(I))/2);   
    lineSeriesI->append(testFor3[0], test3[0]);
    //中间点
    for (int i = 0; i < nL_1; i++) {
        testFor3<<(i*lOfDev+lOfDev/2);
        test3<<*(I+i);
        lineSeriesI->append(testFor3[i+1], test3[i+1]);
    }
    //最后一个点
    testFor3<<nL_1*lOfDev;
    test3<<qAbs(I[nL_1-1]+(I[nL_1-1]-I[nL_1-2])/2);
    lineSeriesI->append(testFor3[nL_1+1], test3[nL_1+1]);

    axisX4I->setRange(testFor3[0], testFor3[nL_1+1]);
    axisY4I->setRange(IMin-(IMax-IMin)*0.1, IMax+(IMax-IMin)*0.1);

    queThread4I->data0(testFor3,test3,IMin-(IMax-IMin)*0.1,0);
    queThread4IChart.data0(testFor3,test3,IMin-(IMax-IMin)*0.1,0);
    chart4I->setTipRange(testFor3[0], testFor3[nL_1+1], IMin-(IMax-IMin)*0.1, IMax+(IMax-IMin)*0.1);
    chartView4I->showPT(powerPos, trainPos,IMin-(IMax-IMin)*0.1);

    pBar->setValue(100);
    windowsTaskbarProgress->setValue(100);
    statusHint->setText("计算完成 ");
    ui->startCal->show();
    ui->pauseCal->hide();
    ui->stopCal->hide();
    pBar->hide();
    windowsTaskbarProgress->hide();
}

void MainWindow::I0Done()
{
    moreChartsFlag = 0;
    widgetI0->hide();
    widgetI1->hide();
    widgetI2->hide();
}

void MainWindow::I1Done(double *I0, double *I1, int nL_1, double lenthOfDev, double I0Min, double I0Max, double I1Min, double I1Max)
{
    moreChartsFlag = 1;
    widgetI0->data(I0, nL_1, lenthOfDev, I0Min, I0Max, 0, powerPos, trainPos);
    widgetI1->data(I1, nL_1, lenthOfDev, I1Min, I1Max, 2, powerPos, trainPos);
    widgetI2->hide();
}

void MainWindow::I2Done(double *I0, double *I1, double *I2, int nL_1, double lenthOfDev, double I0Min, double I0Max, double I1Min, double I1Max, double I2Min, double I2Max)
{
    moreChartsFlag = 2;
    widgetI0->data(I0, nL_1, lenthOfDev, I0Min, I0Max, 0, powerPos, trainPos);
    widgetI1->data(I1, nL_1, lenthOfDev, I1Min, I1Max, 1, powerPos, trainPos);
    widgetI2->data(I2, nL_1, lenthOfDev, I2Min, I2Max, 2, powerPos, trainPos);
    if(widgetI0->isVisible())
        widgetI2->show();
}

void MainWindow::onCalFail(int flag)
{
    switch (flag) {
        case 1:
            statusHint->setText("🤔 计算失败，请将（牵引站）位置设置在给定区间内，或调大区间范围 ");
            dialogWithOneButn -> setMainLabel("🤔 计算失败\n请将（牵引站）位置设置在给定区间内,\n或调大区间范围");
            dialogWithOneButtonAttentionSet();
            break;
        case 2:
            statusHint->setText("🤔 计算失败，请将（负载）位置设置在给定区间内，或调大区间范围 ");
            dialogWithOneButn -> setMainLabel("🤔 计算失败\n请将（负载）位置设置在给定区间内,\n或调大区间范围");
            dialogWithOneButtonAttentionSet();
           break;
        case 3:
            statusHint->setText("🤔 计算失败，划分长度大于区间长度 ");
            dialogWithOneButn -> setMainLabel("🤔 计算失败\n划分长度大于区间长度");
            dialogWithOneButtonAttentionSet();
            break;
        case 4:
            statusHint->setText("🤔 计算失败，两个或多个（牵引站）位于同一位置 ");
            dialogWithOneButn -> setMainLabel("🤔 计算失败\n两个或多个（牵引站）位于同一位置");
            dialogWithOneButtonAttentionSet();
            break;
        case 5:
            statusHint->setText("🤔 计算失败，两个或多个（负载）位于同一位置 ");
            dialogWithOneButn -> setMainLabel("🤔 计算失败\n两个或多个（负载）位于同一位置");
            dialogWithOneButtonAttentionSet();
            break;
        case 6:
            statusHint->setText("🤔 计算失败，（划分长度）不能为0 ");
            dialogWithOneButn -> setMainLabel("🤔 计算失败\n（划分长度）不能为0");
            dialogWithOneButtonAttentionSet();
            break;
        case 7:
            statusHint->setText("🤔 计算失败，（区间长度）不能为0 ");
            dialogWithOneButn -> setMainLabel("🤔 计算失败\n（区间长度）不能为0");
            dialogWithOneButtonAttentionSet();
            break;
        case 8:
            statusHint->setText("🤔 计算失败，电阻不能为0 ");
            dialogWithOneButn -> setMainLabel("🤔 计算失败，电阻不能为0\n计算中需要以电阻作为除数求电导，\n如有特殊需求，\n你可以取一个极小的非零值，\n如0.000001");
            dialogWithOneButtonAttentionSet();
            break;
        case 9:
            statusHint->setText("🤔 计算失败，区间长度过大或划分间隔过小 ");
            dialogWithOneButn -> setMainLabel("🤔 计算失败\n区间长度过大或划分长度过小，\n请调小区间长度，\n或调大划分长度");
            dialogWithOneButtonAttentionSet();
            break;
        case 10:
            statusHint->setText("🤔 计算失败，钢轨接地不在区间内 ");
            dialogWithOneButn -> setMainLabel("🤔 计算失败\n钢轨接地不在区间内，\n请调小钢轨接地位置");
            dialogWithOneButtonAttentionSet();
            break;
    }
    ui->startCal->show();
    ui->pauseCal->hide();
    ui->continueCal->hide();
    ui->stopCal->hide();
    pBar->hide();
    windowsTaskbarProgress->hide();
}

void MainWindow::dialogWithOneButtonAttentionSet()
{
    dialogWithOneButn -> setModal(true);
    dialogWithOneButn -> setWindowIcon(QIcon(":/images/icos/attention2.ico"));
    dialogWithOneButn -> setWindowTitle("注意");
    dialogWithOneButn -> show();
}

void MainWindow::on_pauseCal_clicked()
{
    statusHint->setText("计算线程正在寻找下一个暂停点，界面会有卡顿，请稍等 ");
    ui->pauseCal->hide();
    ui->continueCal->show();
    startcal->pause();
    statusHint->setText("计算已暂停 ");
    pBar->setStyleSheet("QProgressBar::chunk{background: orange;}");
}

void MainWindow::on_continueCal_clicked()
{
    ui->pauseCal->show();
    ui->continueCal->hide();
    startcal->resume();
    statusHint->setText("计算继续中 ");
    pBar->setStyleSheet("QProgressBar::chunk{background: #00b400;}");
}

void MainWindow::on_stopCal_clicked()
{
    statusHint->setText("计算线程正在寻找下一个中止点，请稍等 ");
    pBar->setStyleSheet("QProgressBar::chunk{background: orange;}");
    if(ui->continueCal->isVisible())
        startcal->resume();
    sThread->setData(startcal);
    sThread->start();
    ui->continueCal->setEnabled(false);
    ui->pauseCal->setEnabled(false);
    ui->stopCal->setEnabled(false);
}

void MainWindow::onCalStopped()
{
    ui->startCal->show();
    ui->continueCal->hide();
    ui->pauseCal->hide();
    ui->stopCal->hide();
    ui->continueCal->setEnabled(true);
    ui->pauseCal->setEnabled(true);
    ui->stopCal->setEnabled(true);
    statusHint->setText("计算已中止 ");
    pBar->hide();
    pBar->setStyleSheet("QProgressBar::chunk{background: #00b400;}");
    windowsTaskbarProgress->hide();
}

void MainWindow::calStatus(int status)
{
    pBar->setValue(status);
    windowsTaskbarProgress->setValue(status);
}

void MainWindow::on_showIWidget_clicked()
{
    switch (moreChartsFlag) {
    case -1:
        statusbar2->showMessage("🤔 请计算后重试此功能");
        dialogWithOneButn -> setMainLabel("🤔 你还没有计算过\n请计算后重试此功能");
        dialogWithOneButtonAttentionSet();
        break;
    case 0:
        statusbar2->showMessage("🤔 大地电流与总杂散电流相同");
        dialogWithOneButn -> setMainLabel("🤔 大地电流与总杂散电流相同\n所以不显示了");
        dialogWithOneButtonAttentionSet();
        break;
    case 1:
        widgetI0->show();
        widgetI1->show();
        widgetI2->hide();
        break;
    case 2:
        widgetI0->show();
        widgetI1->show();
        widgetI2->show();
        break;
    }
}

void MainWindow::onpPosReturn(const QString &text)
{
    if(whichP == -1)
        return;
    powerModel->setData0(indexP, 1, text);
    ui->powerTable->setFocus();
    ui->pPos->setFocus();
}

void MainWindow::onpVolReturn(const QString &text)
{
    if(whichP == -1)
        return;
    powerModel->setData0(indexP, 2, text);
    ui->powerTable->setFocus();
    ui->pVol->setFocus();
}

void MainWindow::onpResReturn(const QString &text)
{
    if(whichP == -1)
        return;
    powerModel->setData0(indexP, 3, text);
    ui->powerTable->setFocus();
    ui->pRes->setFocus();
}

void MainWindow::ontPosReturn(const QString &text)
{
    if(whichT == -1)
        return;
    trainModel->setData0(indexT, 1, text);
    ui->trainTable->setFocus();
    ui->tPos->setFocus();
}

void MainWindow::ontCurReturn(const QString &text)
{
    if(whichT == -1)
        return;
    trainModel->setData0(indexT, 2, text);
    ui->trainTable->setFocus();
    ui->tCur->setFocus();
}

void MainWindow::onPDataChange4G(int row, int column, QString str)
{
    if(whichP == row)
        switch (column) {
        case 1:
            ui->pPos->setText(str);
            break;
        case 2:
            ui->pVol->setText(str);
            break;
        case 3:
            ui->pRes->setText(str);
            break;
        }
}

void MainWindow::onTDataChange4G(int row, int column, QString str)
{
    if(whichT == row)
        switch (column) {
        case 1:
            ui->tPos->setText(str);
            break;
        case 2:
            ui->tCur->setText(str);
            break;
        }
}

void MainWindow::on_close0_clicked()
{
    ui->modelWidget0->hide();
    ui->widget4G->hide();
    ui->chartWidget->show();
    ui->widget4exp->show();
}

void MainWindow::on_close1_clicked()
{
    ui->modelWidget1->hide();
    ui->widget4G->hide();
    ui->chartWidget->show();
    ui->widget4exp->show();
}

void MainWindow::on_close2_clicked()
{
    ui->modelWidget2->hide();
    ui->widget4G->hide();
    ui->chartWidget->show();
    ui->widget4exp->show();
}

void MainWindow::on_action_S_2_triggered()
{
    switch (igFlag) {
    case 0:
        ui->chartWidget->hide();
        ui->widget4exp->hide();
        ui->modelWidget1->hide();
        ui->modelWidget2->hide();
        ui->modelWidget0->show();
        ui->widget4G->show();
        break;
    case 1:
        ui->chartWidget->hide();
        ui->widget4exp->hide();
        ui->modelWidget0->hide();
        ui->modelWidget2->hide();
        ui->modelWidget1->show();
        ui->widget4G->show();
        break;
    case 2:
        ui->chartWidget->hide();
        ui->widget4exp->hide();
        ui->modelWidget0->hide();
        ui->modelWidget1->hide();
        ui->modelWidget2->show();
        ui->widget4G->show();
        break;
    }
}

void MainWindow::on_showModel_clicked()
{
    on_action_S_2_triggered();
}
