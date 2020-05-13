#include "stopthread.h"

StopThread::StopThread(QObject *parent) :
    QThread(parent),
    stopped(false)
{

}

void StopThread::setData(calculate *oCal)
{
    cal = oCal;
}

void StopThread::run()
{
    cal->stop();
    cal->wait();
    stopped = false;
    emit calStopped();
}

void StopThread::stop()
{
    stopped = true;
}
