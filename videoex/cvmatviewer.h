#ifndef CVMATVIEWER_H
#define CVMATVIEWER_H

#include <QWidget>
#include <QPainter>
#include <QImage>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class cvMatViewer : public QWidget
{
    Q_OBJECT
public:
    explicit cvMatViewer(QWidget *parent = 0);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    QImage qimg;
    Mat mat;
    
signals:
    
public slots:

    void showImg(Mat img);

protected:
    void paintEvent(QPaintEvent *event);
    
};

#endif // CVMATVIEWER_H
