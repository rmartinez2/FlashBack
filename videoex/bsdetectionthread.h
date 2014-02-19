#ifndef BSDETECTIONTHREAD_H
#define BSDETECTIONTHREAD_H

#include <QThread>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "mat2qimage.h"
#include <QDebug>
#include <QVector>
#include <QImage>


using namespace cv;

class BSDetectionThread: public QThread
{
public:
    BSDetectionThread(QVector<QImage> images);

    QVector<QImage> deter;

    void run();

signals:
    void isBlack(bool);


};

#endif // BSDETECTIONTHREAD_H
