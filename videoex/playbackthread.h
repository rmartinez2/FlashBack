#ifndef PLAYBACKTHREAD_H
#define PLAYBACKTHREAD_H

#include <QThread>
#include <QVector>
#include <QTimer>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/cv.h"
#include <QDebug>
#include "cutratedetectionthread.h"
#include "bsdetectionthread.h"
#include "logodetectionthread.h"

using namespace cv;
using namespace std;

class playBackThread : public QThread
{
    Q_OBJECT
public:
    explicit playBackThread(QVector<Mat> bufferedMats, QObject *parent = 0);
    explicit playBackThread(QObject *parent = 0);
    explicit playBackThread(char* fileName, QObject *parent = 0);

    void run();

    QTimer *frameReady;

    QVector<Mat> buffs;
    QVector<Mat> bsAnalytics;
    QVector<Mat> crAnalytics;
    QVector<Mat> ldAnalytics;
    double FPS;
    char *filePath;
    Size sz;

    CutRateDetectionThread *HCRateThread;
    BSDetectionThread *bSDetectThread;
    logoDetectionThread *logoDectThread;

    VideoCapture cap;





    
signals:
    void sendMat(Mat);
    
public slots:
    void readInBufs(QVector<Mat> buffered);
    void setFPS(double FPS);
    void fromCRate(Mat mat);
    void fromBSDect(Mat mat);
    void fromldDect(Mat mat);
    void queryFrame();
    
};

#endif // PLAYBACKTHREAD_H
