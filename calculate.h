#ifndef CALCULATE_H
#define CALCULATE_H

#include <QThread>
#include <QMutex>

class calculate: public QThread
{
    Q_OBJECT
public:
    explicit calculate(QObject *parent = 0);
    void stop();
    void pause();
    void resume();

    void setPData(QVector<double>, QVector<double>, QVector<double>);
    void setTData(QVector<double>, QVector<double>);
    void setGData(double, double, double, double, double, double, double, double, int);
    void setLData(double, double);
    void setR2eData(double, bool);
protected:

    void run();

    void get_nLn();
    void getA();
    void getT();
    void iniV();
    void solveMatrix();
    void getI();
    void getI0();
    void getI1();
    void getI2();

private:
    int nL;
    int n;

    double *A;
    double *T;
    double *V;
    double vMin;
    double vMax;

    double *I;
    double *I0;
    double *I1;
    double *I2;
    double IMin;
    double IMax;
    double I0Min;
    double I0Max;
    double I1Min;
    double I1Max;
    double I2Min;
    double I2Max;

    struct pData
    {
    double pPos, pVol, pRes;
    } ;
    QList<pData> pD;
    struct tData
    {
    double tPos, tCur;
    } ;
    QList<tData> tD;

    double catenaryRes;
    double railRes;
    double earthRes;
    double transitionRes;
    double drainage;
    double trans2;
    double metal;
    double trans3;
    int igFlag;

    double lenthOfRail;
    double lenthOfDev;

    double r2eP;
    bool wR2e;

    volatile bool stopped;

    QMutex *m_lock;
    int s;

signals:
    void done(double *, int, double, double, double, bool);
    void doneI(double *, int, double, double, double);
    void doneI0();
    void doneI1(double *, double *, int, double, double, double, double, double);
    void doneI2(double *, double *, double *, int, double, double, double, double, double, double, double);
    void notDone(int);
    void status(int);

};

#endif // CALCULATE_H
