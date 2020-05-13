#include "chartview.h"

chartView::chartView(QWidget* parent) :
    QChartView(parent),
    m_tip(NULL){

    setDragMode(QChartView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setMouseTracking(true);
}

chartView::~chartView() {
}

void chartView::slot_showTip(const QPointF& ptAnchor, qreal percent, qreal zeroY, bool bHoverState, int wChart) {
    if (NULL == m_tip) {
        m_tip = new tip(chart());
    }
    if (bHoverState) { // 鼠标悬浮在曲线上
        m_tip->setZeroY(zeroY);
        if(wChart==0)
            m_tip->setText(tr("距离:%1 km\n电流:%2 A").arg(ptAnchor.x()).arg(ptAnchor.y()));
        else
            m_tip->setText(tr("距离:%1 km\n电位:%2 V").arg(ptAnchor.x()).arg(ptAnchor.y()));
        m_tip->setAnchor(ptAnchor);
        m_tip->updateGeometry(percent);
        m_tip->setZValue(chart()->zValue()+10);
        m_tip->show();
    }
    else {
        m_tip->hide();
    }
    viewport()->update();
}

void chartView::showPT(QVector<double> powerPos, QVector<double> trainPos, qreal zeroY)
{
    for(int i=0; i<tP.size(); i++)
        tP[i]->hide();
    for(int i=0; i<tT.size(); i++)
        tT[i]->hide();
    tP.clear();
    tT.clear();

    double temp;
    foreach(temp, powerPos)
    {
        Localtip *m_tP = new Localtip(chart());
        m_tP->setZeroY(zeroY);
        m_tP->setAnchor(QPointF(temp, zeroY));
        m_tP->setColor(QColor(255,0,0));
        m_tP->setZValue(chart()->zValue()+7);
        m_tP->show();
        tP<<m_tP;
    }
    foreach(temp, trainPos)
    {
        Localtip *m_tT = new Localtip(chart());
        m_tT->setZeroY(zeroY);
        m_tT->setAnchor(QPointF(temp, zeroY));
        m_tT->setColor(QColor(0,180,0));
        m_tT->setZValue(chart()->zValue()+8);
        m_tT->show();
        tT<<m_tT;
    }
    viewport()->update();
}

void chartView::setSize(const QSize& szNew) {
    // 有场景及图表时才执行
    if ((NULL != scene()) && (NULL != chart())) {
        // 更新视图尺寸
        resize(szNew);
        // 更新场景尺寸
        scene()->setSceneRect(QRect(QPoint(0, 0), szNew));
        // 更新图表尺寸
        chart()->resize(szNew);
        // 遍历全部tip，更新位置
//        const auto tips = m_tips;
//        for (tip *tip : tips)
//            tip->updateGeometry();

        viewport()->update();
    }
}

void chartView::resizeEvent(QResizeEvent *event) {
    // 使用父对象的尺寸
    setSize(event->size());
    QChartView::resizeEvent(event);
}

//void chartView::slot_keepTip() {
//    m_tips.append(m_tip);
//    m_tip = new tip(chart());
//    m_tip->hide();
//}
