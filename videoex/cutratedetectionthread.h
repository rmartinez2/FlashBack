#ifndef CUTRATEDETECTIONTHREAD_H
#define CUTRATEDETECTIONTHREAD_H

#include <QThread>

#include "mat2qimage.h"
#include <QDebug>
#include <QVector>
#include <QImage>
#include <QtCore>
#include <QColor>
#include <QPixmap>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cmath>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include "opencv2/imgproc/imgproc.hpp"


#define PER1 0
#define PER2 1
#define NOCH -1


using namespace cv;
using namespace std;

class CutRateDetectionThread : public QThread
{
    Q_OBJECT
public:
    explicit CutRateDetectionThread(QVector<QImage> frameSamples,int index, QObject *parent = 0);
    explicit CutRateDetectionThread(QVector<QImage> frameSamples, QObject *parent = 0);
    explicit CutRateDetectionThread(QVector<QPixmap> pixMapSamples, QObject *parent = 0);
    explicit CutRateDetectionThread(QVector<Mat> myMats, QSize frameWH, QObject *parent = 0);
    explicit CutRateDetectionThread(QObject *parent = 0);



    QVector<QImage> mySamples;
    QVector<QPixmap> myPixmaps;
    QVector<Mat> myMats;
    QVector<long long> stdDev2s;
    int gIndex;
    int FPS;
    int FPM;
    int cutsPerSec;
    int seconds;

    QVector<int> currentFrameSums;


    QSize originWindowSz;

    bool isMats;
    bool isPixMaps;

    int currentDev;

    void run();

signals:
    void sendPixMap(QPixmap);
    void sendMats(Mat);
    void highCuts(bool);
public slots:
    QString QImage2String(QImage img);
    QString Mat2String(int frameNumb, Mat mat);
    void getPixelData(QImage img);
    int getPixelData(QPixmap pix);
    int getPixelData(Mat mat, int frame);

    int plusOrMinus(long long stdDevNew, long long stdDevCur, long long diff);
    void setFPS(int FPS);
    void addFrames(QVector<Mat> frames);
    int numbofDigits(int numb);
    //int plusOrMinus(long long stdDevNew, long long stdDevCur, long long diff);
    
};

#endif // CUTRATEDETECTIONTHREAD_H
