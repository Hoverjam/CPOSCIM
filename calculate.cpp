#include "calculate.h"
#include <QDebug>

calculate::calculate(QObject *parent) :
    QThread(parent),
    stopped(false)
{

}

void calculate::run()
{
    m_lock = new QMutex;
    vMin = IMin = I0Min = I1Min = I2Min = 999999;
    vMax = IMax = I0Max = I1Max = I2Max = -999999;

    switch (igFlag) {
    case 0:
        if(catenaryRes == 0 || railRes == 0 || earthRes == 0)
        {
            emit notDone(8);
            stopped = false;
            return;
        }
        break;
    case 1:
        if(catenaryRes == 0 || railRes == 0 || earthRes == 0 || drainage == 0 )
        {
            emit notDone(8);
            stopped = false;
            return;
        }
        break;
    case 2:
        if(catenaryRes == 0 || railRes == 0 || earthRes == 0  || drainage == 0 || metal == 0)
        {
            emit notDone(8);
            stopped = false;
            return;
        }
        break;
    }
    if(lenthOfDev==0)
    {
        emit notDone(6);
        stopped = false;
        return;
    }
    if(lenthOfRail==0)
    {
        emit notDone(7);
        stopped = false;
        return;
    }
    if(lenthOfDev>lenthOfRail)
    {
        emit notDone(3);
        stopped = false;
        return;
    }
    if(r2eP>lenthOfRail&&wR2e==true)
    {
        emit notDone(10);
        stopped = false;
        return;
    }
    switch (igFlag) {
    case 0:
        if(lenthOfRail/14900 >= lenthOfDev) //14900与行数有关
        {
            emit notDone(9);
            stopped = false;
            return;
        }
        break;
    case 1:
        if(lenthOfRail/11110 >= lenthOfDev)
        {
            emit notDone(9);
            stopped = false;
            return;
        }
        break;
    case 2:
        if(lenthOfRail/8940 >= lenthOfDev)
        {
            emit notDone(9);
            stopped = false;
            return;
        }
        break;
    }

    get_nLn();

    pData ip;
    foreach(ip, pD)
    {
        if((int)(ip.pPos/lenthOfDev+0.5)>nL)
        {
            emit notDone(1);
            stopped = false;
            return;
        }
        if(ip.pRes == 0)
        {
            emit notDone(8);
            stopped = false;
            return;
        }
    }
    tData it;
    foreach(it, tD)
        if((int)(it.tPos/lenthOfDev+0.5)>nL)
        {
            emit notDone(2);
            stopped = false;
            return;
        }
    for(int i=0; i<pD.size()-1; i++)
        for(int j=i+1; j<pD.size(); j++)
            if((int)(pD[i].pPos/lenthOfDev+0.5) == (int)(pD[j].pPos/lenthOfDev+0.5))
            {
                emit notDone(4);
                stopped = false;
                return;
            }
    for(int i=0; i<tD.size()-1; i++)
        for(int j=i+1; j<tD.size(); j++)
            if((int)(tD[i].tPos/lenthOfDev+0.5) == (int)(tD[j].tPos/lenthOfDev+0.5))
            {
                emit notDone(5);
                stopped = false;
                return;
            }
    s = n/93;
                                                                                                    emit status(3);
    if(stopped)
    {
        stopped = false;
        return;
    }
    getA();
    getT();
    iniV();
    if(stopped)
    {
        stopped = false;
        return;
    }
    solveMatrix();

    if(stopped)
    {
        stopped = false;
        return;
    }
//    qDebug("%a %a",vMin,vMax);
    emit done(V, nL, lenthOfDev, vMin, vMax, true);

    if(stopped)
    {
        stopped = false;
        return;
    }
    switch (igFlag) {
    case 0:
        getI();
        emit doneI(I, nL-1, lenthOfDev, IMin, IMax);
        emit doneI0();
        break;
    case 1:
        getI0();
        getI1();
        getI();
        emit doneI(I, nL-1, lenthOfDev, IMin, IMax);
        emit doneI1(I0, I1, nL-1, lenthOfDev, I0Min, I0Max, I1Min, I1Max);
        break;
    case 2:
        getI0();
        getI1();
        getI2();
        getI();
        emit doneI(I, nL-1, lenthOfDev, IMin, IMax);
        emit doneI2(I0, I1, I2, nL-1, lenthOfDev, I0Min, I0Max, I1Min, I1Max, I2Min, I2Max);
        break;
    }
                                                                                                        emit status(99);
    stopped = false;
}

void calculate::setPData(QVector<double> oPPos, QVector<double> oPVol, QVector<double> oPRes)
{
    pD.clear();
    pData temp;
    for(int i=0; i<oPPos.size(); i++)
    {
        temp = {oPPos[i],oPVol[i],oPRes[i]};
        pD<<temp;
//        qDebug()<<QString::number(temp.pPos);
    }
}

void calculate::setTData(QVector<double> oTPos, QVector<double> oTCur)
{
    tD.clear();
    tData temp;
    for(int i=0; i<oTPos.size(); i++)
    {
        temp = {oTPos[i],oTCur[i]};
        tD<<temp;
    }
}

void calculate::setGData(double oC, double oR, double oE, double oT, double oD, double oT2, double oM, double oT3, int oFlag)
{
    catenaryRes = oC;
    railRes = oR;
    earthRes = oE;
    transitionRes = oT;
    drainage = oD;
    trans2 = oT2;
    metal = oM;
    trans3 = oT3;
    igFlag = oFlag;
}

void calculate::setLData(double oR, double oD)
{
    lenthOfRail = oR;
//    qDebug()<<QString::number(lenthOfRail);
    lenthOfDev = oD;
}

void calculate::setR2eData(double oR2eP, bool oWR2e)
{
    r2eP = oR2eP;
    wR2e = oWR2e;
}

void calculate::get_nLn()
{
    nL = lenthOfRail/lenthOfDev+1.5;
//    qDebug("%d",nL);
    switch (igFlag) {
    case 0:
        n = nL*3-1;
        break;
    case 1:
        n = nL*4-1;
        break;
    case 2:
        n = nL*5-1;
        break;
    }

}

void calculate::getA()
{
    double *tA = new double[(n+1)*(n+1)] {0};
    if(!stopped)
    {
        m_lock->lock();
        //接触网电导
        for(int i=0; i<(nL-1); i++){
            tA[i*(n+1)+i+1]=tA[(i+1)*(n+1)+i]=-(1/(catenaryRes*lenthOfDev));
            //qDebug("%a",tA[i*(n+1)+i+1]);
        }
        //牵引站过渡电导
        pData temp;
        foreach(temp, pD)
        {
            int pos = (int)(temp.pPos/lenthOfDev+0.5);
            tA[pos*(n+1)+nL+pos]=tA[(nL+pos)*(n+1)+pos]=-(1/temp.pRes);
            //qDebug("%d %a", pos, tA[pos*(n+1)+nL+pos]);
        }
        //钢轨纵向电导
        for(int i=nL; i<(2*nL-1); i++){
            tA[i*(n+1)+i+1]=tA[(i+1)*(n+1)+i]=-(1/(railRes*lenthOfDev));
            //qDebug("%a",tA[i*(n+1)+i+1]);
        }
        m_lock->unlock();
    }
    if(!stopped)
    {
        m_lock->lock();
        switch (igFlag) {
        case 0:
            //大地纵向电导
            for(int i=(2*nL); i<n; i++){
                tA[i*(n+1)+i+1]=tA[(i+1)*(n+1)+i]=-(1/(earthRes*lenthOfDev));
                //qDebug("%a",tA[i*(n+1)+i+1]);
            }
            //钢轨-大地过渡电导
            for(int i=nL; i<(2*nL); i++){
                tA[i*(n+1)+i+nL]=tA[(i+nL)*(n+1)+i]=(-transitionRes*lenthOfDev);
                //qDebug("%a",tA[i*(n+1)+i+nL]);
            }
            if(wR2e)
            {
                int pos = (int)(r2eP/lenthOfDev+0.5);
                tA[(pos+nL)*(n+1)+2*nL+pos]=tA[(2*nL+pos)*(n+1)+(pos+nL)]=-9999999;
            }
            break;
        case 1:
            //排流网纵向电导
            for(int i=(2*nL); i<(3*nL-1); i++){
                tA[i*(n+1)+i+1]=tA[(i+1)*(n+1)+i]=-(1/(drainage*lenthOfDev));
                //qDebug("%a",tA[i*(n+1)+i+1]);
            }
            //大地纵向电导
            for(int i=(3*nL); i<n; i++){
                tA[i*(n+1)+i+1]=tA[(i+1)*(n+1)+i]=-(1/(earthRes*lenthOfDev));
                //qDebug("%a",tA[i*(n+1)+i+1]);
            }
            //钢轨-排流网
            for(int i=nL; i<(2*nL); i++){
                tA[i*(n+1)+i+nL]=tA[(i+nL)*(n+1)+i]=(-transitionRes*lenthOfDev);
                //qDebug("%a",tA[i*(n+1)+i+nL]);
            }
            //排流网-大地
            for(int i=(2*nL); i<(3*nL); i++){
                tA[i*(n+1)+i+nL]=tA[(i+nL)*(n+1)+i]=(-trans2*lenthOfDev);
                //qDebug("%a",tA[i*(n+1)+i+nL]);
            }
            if(wR2e)
            {
                int pos = (int)(r2eP/lenthOfDev+0.5);
                tA[(pos+nL)*(n+1)+3*nL+pos]=tA[(3*nL+pos)*(n+1)+(pos+nL)]=-9999999;
            }
            break;
        case 2:
            //排流网纵向电导
            for(int i=(2*nL); i<(3*nL-1); i++){
                tA[i*(n+1)+i+1]=tA[(i+1)*(n+1)+i]=-(1/(drainage*lenthOfDev));
                //qDebug("%a",tA[i*(n+1)+i+1]);
            }
            //埋地金属纵向电导
            for(int i=(3*nL); i<(4*nL-1); i++){
                tA[i*(n+1)+i+1]=tA[(i+1)*(n+1)+i]=-(1/(metal*lenthOfDev));
                //qDebug("%a",tA[i*(n+1)+i+1]);
            }
            //大地纵向电导
            for(int i=(4*nL); i<n; i++){
                tA[i*(n+1)+i+1]=tA[(i+1)*(n+1)+i]=-(1/(earthRes*lenthOfDev));
                //qDebug("%a",tA[i*(n+1)+i+1]);
            }
            //钢轨-排流网
            for(int i=nL; i<(2*nL); i++){
                tA[i*(n+1)+i+nL]=tA[(i+nL)*(n+1)+i]=(-transitionRes*lenthOfDev);
                //qDebug("%a",tA[i*(n+1)+i+nL]);
            }
            //排流网-埋地金属
            for(int i=(2*nL); i<(3*nL); i++){
                tA[i*(n+1)+i+nL]=tA[(i+nL)*(n+1)+i]=(-trans2*lenthOfDev);
                //qDebug("%a",tA[i*(n+1)+i+nL]);
            }
            //埋地金属-大地
            for(int i=(3*nL); i<(4*nL); i++){
                tA[i*(n+1)+i+nL]=tA[(i+nL)*(n+1)+i]=(-trans3*lenthOfDev);
                //qDebug("%a",tA[i*(n+1)+i+nL]);
            }
            if(wR2e)
            {
                int pos = (int)(r2eP/lenthOfDev+0.5);
                tA[(pos+nL)*(n+1)+4*nL+pos]=tA[(4*nL+pos)*(n+1)+(pos+nL)]=-999999;
            }
        }
        m_lock->unlock();
    }
                                                                                                        emit status(4);
    if(!stopped)
    {
        //自导
        for(int i=0; i<(n+1); i++)
        {
            m_lock->lock();
            for(int j=0; j<i; j++)//{
                tA[i*(n+1)+i]+=-tA[i*(n+1)+j];
                //qDebug("%d %d %a",i,j,tA[i*(n+1)+i]);}
            for(int j=i+1; j<(n+1); j++)//{
                tA[i*(n+1)+i]+=-tA[i*(n+1)+j];
                //qDebug("%d %d %a",i,j,tA[i*(n+1)+i]);}
            m_lock->unlock();
        }

//    for(int i=0; i<n+1; i++)
//    {
//        for(int j=0; j<n+1; j++)
//            qDebug("%d %d %a",i,j,tA[i*(n+1)+j]);
//    }
    }
                                                                                                       emit status(5);
    if(!stopped)
    {
        int tN;
        switch (igFlag) {
        case 0:
            tN = 2;
            break;
        case 1:
            tN = 3;
            break;
        case 2:
            tN = 4;
            break;
        }
        A = new double[n*n] {0};
        for(int i=0; i<tN*nL; i++)
        {
            m_lock->lock();
            for(int j=0; j<tN*nL; j++)
                A[i*n+j] = tA[i*(n+1)+j];
            m_lock->unlock();
        }
        for(int i=0; i<tN*nL; i++)
        {
            m_lock->lock();
            for(int j=tN*nL; j<n; j++)
                A[i*n+j] = tA[i*(n+1)+j+1];
            m_lock->unlock();
        }
        for(int i=tN*nL; i<n; i++)
        {
            m_lock->lock();
            for(int j=0; j<tN*nL; j++)
                A[i*n+j] = tA[(i+1)*(n+1)+j];
            m_lock->unlock();
        }
        for(int i=tN*nL; i<n; i++)
        {
            m_lock->lock();
            for(int j=tN*nL; j<n; j++)
                A[i*n+j] = tA[(i+1)*(n+1)+j+1];
            m_lock->unlock();
        }

//    for(int i=0; i<n; i++)
//    {
//        for(int j=0; j<n; j++)
//            qDebug("%d %d %a",i,j,A[i*n+j]);
//        qDebug()<<"\n";
//    }
    }
    delete[]tA;
}

void calculate::getT()
{
    if(!stopped)
    {
        T = new double[n] {0};
        //牵引站
        pData ip;
        foreach(ip, pD)
        {
            m_lock->lock();
            int pos = (int)(ip.pPos/lenthOfDev+0.5);
            T[pos] = ip.pVol/ip.pRes;
            T[pos+nL] = -T[pos];
            m_lock->unlock();
        }
        //负载
        tData it;
        foreach(it, tD)
        {
            m_lock->lock();
            int pos = (int)(it.tPos/lenthOfDev+0.5);
            T[pos] += -it.tCur;
            T[pos+nL] = -T[pos];
            m_lock->unlock();
        }

    }
}

void calculate::iniV()
{
    V = new double[n] {0};
}

void calculate::solveMatrix()//v的最大最小值要加进去
{
    double *L = new double[n*n];//开辟L矩阵空间
    double *U = new double[n*n];//开辟U矩阵空间
    double *y = new double[n];//开辟y矩阵空间
    if(!stopped)
    {
        for (int i=0; i<n; i++)
        {
            m_lock->lock();
            for (int j=0; j<n; j++)
            {

                *(U+i*n+j) = 0;//暂时全部赋值为0
                if (i==j)
                    *(L+i*n+j) = 1;//对角线赋值为1
                else
                    *(L+i*n+j) = 0;//其他暂时赋值为0
            }
            m_lock->unlock();
        }
    }
    if(!stopped)
    {
    int i = 0;
    for (int k=0; k<n; k++)//计算u和l矩阵的值
    {
        if(!stopped)
        {
            for (int j=k; j<n; j++)
            {
                m_lock->lock();
                *(U+k*n+j) = *(A+k*n+j);//第一行
                for (int r=0; r<k; r++)//接下来由L的前一列算u的下一行
                    *(U+k*n+j) = *(U+k*n+j) - (*(L+k*n+r)*(*(U+r*n+j)));
                m_lock->unlock();
            }
        }
        if(!stopped)
        {
            for (int i=k+1; i<n; i++)//计算L的列
            {
                m_lock->lock();
                *(L+i*n+k) = *(A+i*n+k);
                for (int r=0; r<k; r++)
                    *(L+i*n+k) = *(L+i*n+k) - (*(L+i*n+r)*(*(U+r*n+k)));
                *(L+i*n+k) = *(L+i*n+k) / (*(U+k*n+k));
                m_lock->unlock();
            }
        }
                                                                                                if(s!=0&&k%s==0)
                                                                                                {
                                                                                                    emit status(6+i);
                                                                                                    i++;
                                                                                                }
    }
    }
    if(!stopped)
    {
        for (int i=0; i<n; i++)//由Ly=b算y
        {
            m_lock->lock();
            *(y+i) = *(T+i);
            for (int j=0; j<i; j++)
                *(y+i) = *(y+i) - *(L+i*n+j)*(*(y+j));
            m_lock->unlock();
        }
    }
    if(!stopped)
    {
        for (int i=n-1; i>=0; i--)//由Ux=y算x
        {
            m_lock->lock();
            *(V+i) = *(y +i);
            for (int j=i+1; j<n; j++)
                *(y+i) = *(y+i) - *(U+i*n+j)*(*(V+j));
            *(V+i) = *(y+i) / (*(U+i*n+i));

            if(i>=nL && i<=2*nL && *(V+i)<=vMin)
                vMin = *(V+i);
            if(i>=nL && i<=2*nL && *(V+i)>=vMax)
                vMax = *(V+i);
//            qDebug("%d  %a",i,*(V+i));
            m_lock->unlock();
        }
    }
    delete[]L;//释放空间
    delete[]U;
    delete[]y;
}

void calculate::getI0()
{
    I0 = new double[nL-1] {0};
        for(int i=0; i<nL-1; i++)
        {
            *(I0+i) = qAbs((*(V+2*nL+i+1)-*(V+2*nL+i))/(drainage*lenthOfDev));
            if(*(I0+i)<=I0Min)
                I0Min = *(I0+i);
            if(*(I0+i)>=I0Max)
                I0Max = *(I0+i);
            //qDebug("%d  %a",i,*(I0+i));
        }
}

void calculate::getI1()
{
    I1 = new double[nL-1] {0};
    if(igFlag == 1)
    {
        *I1 = qAbs(*(V+3*nL)/(earthRes*lenthOfDev));
        for(int i=0; i<nL-2; i++)
        {
            *(I1+i+1) = qAbs((*(V+3*nL+i+1)-*(V+3*nL+i))/(earthRes*lenthOfDev));
            if(*(I1+i+1)<=I1Min)
                I1Min = *(I1+i+1);
            if(*(I1+i+1)>=I1Max)
                I1Max = *(I1+i+1);
        }
    }
    else if(igFlag == 2)
        for(int i=0; i<nL-1; i++)
        {
            *(I1+i) = qAbs((*(V+3*nL+i+1)-*(V+3*nL+i))/(metal*lenthOfDev));
            if(*(I1+i)<=I1Min)
                I1Min = *(I1+i);
            if(*(I1+i)>=I1Max)
                I1Max = *(I1+i);
        }
}

void calculate::getI2()
{
    I2 = new double[nL-1] {0};
    *I2 = qAbs(*(V+4*nL)/(earthRes*lenthOfDev));
    for(int i=0; i<nL-2; i++)
    {
        *(I2+i+1) = qAbs((*(V+4*nL+i+1)-*(V+4*nL+i))/(earthRes*lenthOfDev));
        if(*(I2+i+1)<=I2Min)
            I2Min = *(I2+i+1);
        if(*(I2+i+1)>=I2Max)
            I2Max = *(I2+i+1);
    }
}

void calculate::getI()
{
    I = new double[nL-1] {0};
    switch (igFlag) {
    case 0:
        *I = qAbs(*(V+2*nL)/(earthRes*lenthOfDev));
        for(int i=0; i<nL-2; i++)
        {
            *(I+i+1) = qAbs((*(V+2*nL+i+1)-*(V+2*nL+i))/(earthRes*lenthOfDev));
            if(*(I+i+1)<=IMin)
                IMin = *(I+i+1);
            if(*(I+i+1)>=IMax)
                IMax = *(I+i+1);
        }
        break;
    case 1:
        for(int i=0; i<nL-1; i++)
        {
            *(I+i) = *(I0+i) + *(I1+i);
            if(*(I+i)<=IMin)
                IMin = *(I+i);
            if(*(I+i)>=IMax)
                IMax = *(I+i);
        }
        break;
    case 2:
        for(int i=0; i<nL-1; i++)
        {
            *(I+i) = *(I0+i) + *(I1+i) + *(I2+i);
            if(*(I+i)<=IMin)
                IMin = *(I+i);
            if(*(I+i)>=IMax)
                IMax = *(I+i);
        }
    }

}

void calculate::pause()
{
    m_lock->lock();
}

void calculate::resume()
{
    m_lock->unlock();
}

void calculate::stop()
{
    stopped = true;
}
