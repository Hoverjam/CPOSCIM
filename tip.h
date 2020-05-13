#ifndef TIP_H
#define TIP_H

#include <QtCharts>
#include <QGraphicsItem>
#include <QFont>

QT_CHARTS_USE_NAMESPACE

class tip : public QObject,public QGraphicsItem
{
    Q_OBJECT

public:
    tip (QChart* chart);

    void setText(const QString &text); //消息框显示内容
    void setAnchor(QPointF point); //消息框左下角坐标
    void updateGeometry(qreal percent);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setZeroY(qreal);

private:
    QString m_text;
    QRectF m_textRect;
    QRectF m_rect;
    QPointF m_anchor;
    QFont m_font;
    QChart *m_chart;
    qreal zeroY;
};

#endif // TIP_H
