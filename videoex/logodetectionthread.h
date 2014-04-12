#ifndef LOGODETECTIONTHREAD_H
#define LOGODETECTIONTHREAD_H

#include <QThread>
#include <QImage>
#include <QtCore>
#include <QVector>

#include "iostream"



#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/gpu/gpu.hpp"

using namespace cv;
using namespace std;


class logoDetectionThread : public QThread
{
    Q_OBJECT
public:
    explicit logoDetectionThread(QVector<QImage> samples, QObject *parent = 0);
    explicit logoDetectionThread(QObject *parent = 0);

    void run();

    QVector<Mat> myMats;
    Mat logoMat;
    Mat logoGray, gradX, gradY, absGradX, absGradY, gradLogo, gradTemp;

    int x1;
    int x2;
    int y1;
    int y2;
    int scale;
    int delta;
    int ddepth;
    
signals:
    void ldSendMat(Mat myMats);
    void noLogo(bool);
    
public slots:

    void addFrames(QVector<Mat> frames);
    void setLogoLoc(int x1, int y1, int x2, int y2);

    
};

#endif // LOGODETECTIONTHREAD_H
