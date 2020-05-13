#include "queryresultthread.h"
#include "dialogwithonebutn.h"

QueryResultThread::QueryResultThread(QObject *parent) :
    QThread(parent),
    stopped(false)
{

}

void QueryResultThread::run()
{
    int first = 0;
    int last = distance.size();//不-1避开末尾死区
    int i;
    while(true)
    {
        i=(first+last)/2;
        if(queryD==distance[i])
        {
            emit done(QPointF(queryD,current[i]), percent, zeroY, true, wChart);
            goto finish;
        }
        else if(queryD > distance[i] && queryD < distance[i+1])
        {
            emit done(QPointF(queryD,current[i+1]-(distance[i+1]-queryD)/(distance[i+1]-distance[i])*(current[i+1]-current[i])), percent, zeroY, true, wChart);
            goto finish;
        }
        else if(queryD < distance[i] && queryD > distance[i-1])
        {
            emit done(QPointF(queryD,current[i]-(distance[i]-queryD)/(distance[i]-distance[i-1])*(current[i]-current[i-1])), percent, zeroY, true, wChart);
            goto finish;
        }
        else if(queryD > distance[i])
            first=i;
        else
            last=i;
    }
    finish:

//    QVector<qreal>::iterator it;
//    QVector<qreal>::iterator begin = distance.begin();
//    for(it=distance.begin(); *it<queryD; ++it)
//    {
//    }
//    qreal temp;
//    if (queryD < *it)
//        //两点间线性差值
//        temp = current[std::distance(begin,it)]-(*it-queryD)/(*it-*(it-1))*(current[std::distance(begin,it)]-current[std::distance(begin,it-1)]);
//    else
//        temp = current[std::distance(begin,it)];
//    emit done(QPointF(queryD,temp), percent, true);

    stopped = false;
}

void QueryResultThread::stop()
{
    stopped = true;
}

void QueryResultThread::data0(const QVector<qreal> &disOut, const QVector<qreal> &curOut,qreal oZeroY, int oChart)
{
    distance = disOut;
    current = curOut;
    zeroY = oZeroY;
    wChart = oChart;
}

void QueryResultThread::data(const qreal whether, const qreal &queryData)
{
    percent = whether;
    queryD = queryData;
}
