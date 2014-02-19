#ifndef QCONDITION_H
#define QCONDITION_H

#include <QtCore>
#include <QThread>
#include <QMutex>
#include <QSemaphore>

class QCondition: public QObject
{
    Q_OBJECT

public:
    QCondition(QObject *parent = 0);


    QMutex lock;
    int waiting;
    int sigs;
    QSemaphore *waitSem;
    QSemaphore *waitDone;

public slots:

    void conditionSignal();
    void conditionBroadcast();
   // int conditionWaitTimeOut(QMutex mutex);
    //int conditionWait(QMutex mutex);


};

#endif // QCONDITION_H
