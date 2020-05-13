#ifndef QUERYRESULTTHREAD_H
#define QUERYRESULTTHREAD_H

#include <QThread>

class QueryResultThread: public QThread
{
    Q_OBJECT

public:
    explicit QueryResultThread(QObject *parent = 0);
    void stop();

    void data0(const QVector<qreal> &disOut, const QVector<qreal> &curOut, qreal, int);
    void data(const qreal whether, const qreal &queryData);
    void justRun(){run();}

protected:
    void run();

private:
    volatile bool stopped;

    QVector<qreal> distance;
    QVector<qreal> current;
    qreal percent;
    qreal queryD;
    qreal wChart;

    qreal zeroY;

signals:
    void done(const QPointF&, qreal, qreal, bool, int);
};

#endif // QUERYRESULTTHREAD_H
