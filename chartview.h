#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts>
#include <QWidget>
#include <QGraphicsView>
#include <QChartView>

#include "tip.h"
#include "localtip.h"

QT_CHARTS_USE_NAMESPACE

class chartView: public QChartView {
    Q_OBJECT
public:
    chartView(QWidget* parent=NULL);
    virtual ~chartView();

    void showPT(QVector<double>, QVector<double>, qreal);

public slots:
    void slot_showTip(const QPointF&,qreal,qreal, bool, int);


protected:
    void setSize(const QSize&);
    void resizeEvent(QResizeEvent *);

private:
    tip * m_tip;   // tip对象
    QList<Localtip *> tP;
    QList<Localtip *> tT;
};

#endif // CHARTVIEW_H
