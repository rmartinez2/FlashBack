#ifndef CVMATVIEWER_H
#define CVMATVIEWER_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <QDebug>
#include <QTimer>

#include<stdlib.h>

#include "pixeldatathread.h"

using namespace cv;
using namespace std;

class cvMatViewer : public QWidget
{
    Q_OBJECT
public:
    explicit cvMatViewer(QWidget *parent = 0);

    ~cvMatViewer();
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    QImage qimg;
    Mat mat;

    bool playing;

    int height;
    int width;
    int step;
    int channels;

    QList<Mat> holder;
    QVector<Mat> rewindHolder;

    //std::vector<Mat> test;

    QTimer *pausedTimer;

    pixelDataThread *dataCpy;

    bool isPIP;
    
signals:
    
public slots:

    void showImg(const Mat &img);
    void isPaused();
    void isPlaying();
    void pbAfterPause();


protected:
    void paintEvent(QPaintEvent *event);

    
};

#endif // CVMATVIEWER_H
