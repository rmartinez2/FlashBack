#include "qcondition.h"

QCondition::QCondition(QObject *parent):
    QObject(parent)
{
    waitSem = new QSemaphore(0);
    waitDone = new QSemaphore(0);
    waiting = 0;
    sigs = 0;

    if(!waitSem || !waitDone)
        qDebug() << "Error creating QCondition object";
}

void QCondition::conditionSignal()
{
    if(!this){
        qDebug() << "QCondition Variable is null";
    }
    else{
        this->lock.lock();

        if(this->waiting > this->sigs){
            ++this->sigs;

            //SemPost
            this->waitSem->release(waitSem->available());

            this->lock.unlock();

            //SemWait
            this->waitDone->tryAcquire(waitDone->available());

        }else{
            this->lock.unlock();
        }
    }
}

void QCondition::conditionBroadcast()
{

    this->lock.lock();

    if(this->waiting > this->sigs){
        int numWaiting;

        numWaiting = this->waiting - this->sigs;

        for(int i = 0; i < numWaiting; i++){
            this->waitSem->release(waitSem->available());
        }

        this->lock.unlock();

        for(int i = 0; i < numWaiting; i++){
            this->waitDone->tryAcquire(waitDone->available());
        }

    }else{
        this->lock.unlock();
    }

}

/*int QCondition::conditionWaitTimeOut(QMutex mutex)
{
    int retval;

    this->lock.lock();
    ++this->waiting;
    this->lock.unlock();

    mutex.unlock();

    return retval;
}


int QCondition::conditionWait(QMutex mutex)
{
    return conditionWaitTimeOut(mutex);
}*/


