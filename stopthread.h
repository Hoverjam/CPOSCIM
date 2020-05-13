#ifndef STOPTHREAD_H
#define STOPTHREAD_H
#include <QThread>
#include <calculate.h>

class StopThread: public QThread
{
    Q_OBJECT
public: 
    explicit StopThread(QObject *parent = 0);
    void stop();

    void setData(calculate *);

protected:
    void run();

signals:
    void calStopped();

private:
    volatile bool stopped;

    calculate *cal;
};

#endif // STOPTHREAD_H
