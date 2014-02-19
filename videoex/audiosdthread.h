#ifndef AUDIOSDTHREAD_H
#define AUDIOSDTHREAD_H

#include <QThread>

class AudioSDThread : public QThread
{
    Q_OBJECT
public:
    explicit AudioSDThread(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // AUDIOSDTHREAD_H
