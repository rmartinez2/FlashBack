#ifndef BSDETECTIONTHREAD_H
#define BSDETECTIONTHREAD_H

#include <QThread>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "mat2qimage.h"
#include <QDebug>
#include <QVector>
#include <QImage>
#include <QList>
#include <iostream>


using namespace cv;
using namespace std;

class BSDetectionThread: public QThread
{

    Q_OBJECT

public:
    explicit BSDetectionThread(QList<Mat> images, QObject *parent = 0);
    explicit BSDetectionThread(QObject *parent = 0);

    QList<Mat> deter;

    void run();

signals:
    void isBlack(bool);
    void sendMyMat(Mat);

public slots:
    void readInFrames(QList<Mat> mats);


};

#endif // BSDETECTIONTHREAD_H
