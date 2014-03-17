#ifndef LOGODETECTIONTHREAD_H
#define LOGODETECTIONTHREAD_H

#include <QThread>
#include <QImage>
#include <QtCore>

extern "C"{
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
}

class logoDetectionThread : public QThread
{
    Q_OBJECT
public:
    explicit logoDetectionThread(QVector<QImage> samples, QObject *parent = 0);

    void run();
    
signals:
    
public slots:
    
};

#endif // LOGODETECTIONTHREAD_H
