#ifndef FRAMEGLWIDGET_H
#define FRAMEGLWIDGET_H


#include <qgl.h>

#include "opencv/cv.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QtCore>
#include <QtDebug>
#include <QImage>
#include <QColor>

using namespace cv;
using namespace std;


class FrameGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit FrameGLWidget(const char* path, QWidget *parent = 0);

private:

    const char* vidPath;

    bool mSceneChanged; //Indicates when frame needs to be redrawn

    QImage mRenderQtImg; //Image that needs to be rendered
    Mat mOriginImg;     // Original img

    QColor mBgColor; //Background color

    int mOutH;      //Resized img height
    int mOutW;      //Resized img width
    float mImgRatio; //Ratio between h/w

    int mPosX; //Top left x coord
    int mPosY; //Top left y coord


    
signals:

    void imageSizeChanged(int outW, int outH);
    
public slots:

    bool showImage(Mat img);
    void playVid();

protected:

   virtual void initializeGL();
   virtual void resizeGL(int w, int h);
   virtual void paintGL();

    void updateScene();
    void renderImage();
    
};

#endif // FRAMEGLWIDGET_H
