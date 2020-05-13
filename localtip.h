#ifndef LOCALTIP_H
#define LOCALTIP_H

#include <QtCharts>
#include <QGraphicsItem>

QT_CHARTS_USE_NAMESPACE

class Localtip : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Localtip(QChart* chart);

    void setZeroY(qreal);
    void setAnchor(QPointF point); //左下角坐标
    void setColor(QColor);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QColor tColor;
    QPointF m_anchor;
    QChart *m_chart;
    qreal zeroY;
};

#endif // LOCALTIP_H
