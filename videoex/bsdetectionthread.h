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
#include <iostream>


using namespace cv;
using namespace std;

class BSDetectionThread: public QThread
{

    Q_OBJECT

public:
    explicit BSDetectionThread(QVector<Mat> images, QObject *parent = 0);
    explicit BSDetectionThread(QObject *parent = 0);

    QVector<Mat> deter;

    Point p1;
    Point p2;

    void run();

signals:
    void isBlack(bool);
    void sendMyMat(Mat);

public slots:
    void readInFrames(QVector<Mat> mats);
    void readInPointVals(Point p1, Point p2);


};

#endif // BSDETECTIONTHREAD_H
