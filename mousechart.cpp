#include "mousechart.h"
#include <QDebug>
#include <QPointF>

MouseChart::MouseChart()
{
    setAcceptHoverEvents(true);
    QPen pn(QColor(0,120,215),2);
    setBackgroundPen(pn);
}

void MouseChart::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF a = QChart::mapToValue(QPointF(event->pos().x(),event->pos().y()));

    if(a.x()>=xl && a.x()<=xh && a.y()>=yl && a.y()<=yh)
        emit hover(a,true);
    else
        emit hover(a,false);
}

void MouseChart::setTipRange(double oxl, double oxh, double oyl, double oyh)
{
    xl = oxl;
    xh = oxh;
    yl = oyl;
    yh = oyh;
}
