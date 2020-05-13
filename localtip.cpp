#include "localtip.h"

Localtip::Localtip(QChart *chart):
    QGraphicsItem(chart),
    m_chart(chart)
{

}

void Localtip::setZeroY(qreal oZeroY){
    zeroY = oZeroY;
}

void Localtip::setAnchor(QPointF point)
{
    m_anchor = point;
}
void Localtip::setColor(QColor oColor)
{
    tColor = oColor;
}

QRectF Localtip::boundingRect() const
{
    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));
    QRectF rect;
    rect.setLeft(anchor.x());
    rect.setRight(anchor.x()+5);
    rect.setTop(anchor.y()-20);
    rect.setBottom(anchor.y());
    return rect;
}

void Localtip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));
    QPointF anchorZero = mapFromParent(m_chart->mapToPosition(QPointF(0,zeroY)));

    QLinearGradient Linearx(anchor.x()-2,anchorZero.y(),anchor.x()-2,anchorZero.y()-20);
    Linearx.setColorAt(0,tColor);
    Linearx.setColorAt(1,QColor(255,255,255,0));

    painter->setPen(Qt::transparent);
    painter->setBrush(Linearx);
    painter->drawRect(anchor.x()-2,anchorZero.y(),5,20-anchorZero.y());
}
