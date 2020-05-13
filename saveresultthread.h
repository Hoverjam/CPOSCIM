#ifndef SAVERESULTTHREAD_H
#define SAVERESULTTHREAD_H

#include <QThread>

class SaveResultThread: public QThread
{
    Q_OBJECT

public:
    explicit SaveResultThread(QObject *parent = 0);
    void stop();

    void data(const QVector<qreal> &disOut ,const QVector<qreal> &curOut ,const QString &fileNameOut, int oFlag)
    {
        distance = disOut;
        current = curOut;
        fileName = fileNameOut;
        flag = oFlag;
    }

signals:
    void saved(QString);

protected:
    void run();
private:
    volatile bool stopped;

    QVector<qreal> distance;
    QVector<qreal> current;
    QString fileName;
    int flag;
};

#endif // SAVERESULTTHREAD_H
