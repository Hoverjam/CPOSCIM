#include "tip.h"
#include <QDebug>

tip::tip(QChart *chart):
   QGraphicsItem(chart),
   m_chart(chart)
{
}
void tip::setZeroY(qreal oZeroY){
    zeroY = oZeroY;
}

void tip::setText(const QString &text)
{
    m_text = text;
    m_font.setFamily(tr("微软雅黑"));
    QFontMetrics metrics(m_font);
    m_textRect = metrics.boundingRect(QRect(0, 0, 1, 1), Qt::AlignLeft, m_text);
    m_textRect.translate(5,5);
    prepareGeometryChange();
    m_rect = m_textRect.adjusted(-5, -5, 5, 5);
}

void tip::setAnchor(QPointF point)
{
    m_anchor = point;
}

void tip::updateGeometry(qreal percent)
{
    prepareGeometryChange();
    if(percent<0.85)
        setPos(m_chart->mapToPosition(m_anchor) + QPoint(0, -60)); //设置tip的位置
    else
        setPos(m_chart->mapToPosition(m_anchor) + QPoint(-m_rect.width(), -60));
}

QRectF tip::boundingRect() const
{
    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));
    QRectF rect;
    rect.setLeft(qMin(m_rect.left(), anchor.x()));
    rect.setRight(qMax(m_rect.right(), anchor.x()));
    rect.setTop(qMin(m_rect.top(), anchor.y()));
    rect.setBottom(qMax(m_rect.bottom(), anchor.y()));
    return rect;
}

void tip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPainterPath path;
    path.addRoundedRect(m_rect, 6, 6); //后两位以矩形宽度的百分比指定圆角大小

    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));
    //qDebug()<<m_chart->mapToPosition(m_anchor).x()<<" "<<m_chart->mapToPosition(m_anchor).y();
    //qDebug()<<anchor.x()<<" "<<anchor.y();
    if (!m_rect.contains(anchor))
    {
        QPointF point1, point2;

        bool above = anchor.y() <= m_rect.top();
        bool aboveCenter = anchor.y() > m_rect.top() && anchor.y() <= m_rect.center().y();
        bool belowCenter = anchor.y() > m_rect.center().y() && anchor.y() <= m_rect.bottom();
        bool below = anchor.y() > m_rect.bottom();

        bool onLeft = anchor.x() <= m_rect.left();
        bool leftOfCenter = anchor.x() > m_rect.left() && anchor.x() <= m_rect.center().x();
        bool rightOfCenter = anchor.x() > m_rect.center().x() && anchor.x() <= m_rect.right();
        bool onRight = anchor.x() > m_rect.right();

        //获得最近的矩形角，下面这段给圆角矩形加了一个指向的角
        qreal x = (onRight + rightOfCenter) * m_rect.width();
        qreal y = (below + belowCenter) * m_rect.height();
        bool cornerCase = (above && onLeft) || (above && onRight) || (below && onLeft) || (below && onRight);
        bool vertical = qAbs(anchor.x() - x) > qAbs(anchor.y() - y);

        qreal x1 = x + leftOfCenter * 10 - rightOfCenter * 20 + cornerCase * !vertical * (onLeft * 10 - onRight * 20);
        qreal y1 = y + aboveCenter * 10 - belowCenter * 20 + cornerCase * vertical * (above * 10 - below * 20);;
        point1.setX(x1);
        point1.setY(y1);

        qreal x2 = x + leftOfCenter * 20 - rightOfCenter * 10 + cornerCase * !vertical * (onLeft * 20 - onRight * 10);;
        qreal y2 = y + aboveCenter * 20 - belowCenter * 10 + cornerCase * vertical * (above * 20 - below * 10);;
        point2.setX(x2);
        point2.setY(y2);

        path.moveTo(point1);
        path.lineTo(anchor);
        path.lineTo(point2);
        path = path.simplified();
    }

    QPen pn(Qt::lightGray,2,Qt::SolidLine);
    painter->setPen(pn);
    painter->setBrush(Qt::white);
    painter->drawPath(path);

    painter->setPen(Qt::black);
    painter->setFont(tr("微软雅黑"));
    painter->drawText(m_textRect, m_text);

    //画位置指向
    QPointF anchorZero = mapFromParent(m_chart->mapToPosition(QPointF(0,zeroY)));

    QLinearGradient Linearx(anchor.x()-2,anchorZero.y(),anchor.x()-2,anchor.y());
    Linearx.setColorAt(0,QColor(255,170,0));
    Linearx.setColorAt(0.8,QColor(255,255,255,0));
    QLinearGradient Lineary(anchorZero.x(),anchor.y()-2,anchor.x(),anchor.y()-2);
    Lineary.setColorAt(0,QColor(255,170,0));
    Lineary.setColorAt(0.8,QColor(255,255,255,0));

    painter->setPen(Qt::transparent);
    painter->setBrush(Linearx);
    painter->drawRect(anchor.x()-2,anchorZero.y(),5,anchor.y()-anchorZero.y());
    painter->setBrush(Lineary);
    painter->drawRect(anchorZero.x(),anchor.y()-2,anchor.x()-anchorZero.x(),5);

}
