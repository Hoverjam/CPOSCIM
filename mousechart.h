#ifndef MOUSECHART_H
#define MOUSECHART_H
#include <QtCharts>


class MouseChart : public QChart
{
    Q_OBJECT

public:
    MouseChart();

    void setTipRange(double, double, double, double);

signals:
    void hover(const QPointF&, bool);

private:
    double xl,xh,yl,yh;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // MOUSECHART_H
