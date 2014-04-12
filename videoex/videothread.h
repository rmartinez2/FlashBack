#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QtCore>
#include <QDebug>
#include <QThread>
#include <QByteArray>
#include <QBuffer>
#include <QPixmap>
#include <QRgb>
#include <QVector>
#include <QMutexLocker>
#include <QImageReader>
#include <QtOpenGL/QGLWidget>



extern "C"{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/imgutils.h"
    #include "libavutil/opt.h"
    #include "libswscale/swscale.h"
    #include "libavformat/avio.h"
    #include "libavutil/avstring.h"
    #include "libavutil/time.h"
    #include "libswresample/swresample.h"
}

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/cv.h"

#include "mat2qimage.h"
#include "cutratedetectionthread.h"
#include "playbackthread.h"

using namespace cv;

class VideoThread : public QThread
{
    Q_OBJECT

public:
    explicit VideoThread(AVFormatContext *formCtx, playBackThread *thread ,QObject *parent = 0);

    void run();

    AVPacket packet;
    AVFrame *vFrame, *vFrameRGB, *bgrFrame;
    AVFormatContext *formCtx;
    QBuffer vBuf;
    QByteArray vData;
    AVStream *videostream;
    AVCodecContext *vCodecCtx = NULL;
    AVCodec *vCodec = NULL;
    AVDictionary *cDict = NULL;
    SwsContext *imgConvertCtx;
    int vidStream;
    int destWidth, destHeight, destFmt;
    QVector <QImage>  qVec;
    QVector <QPixmap> pVec;
    QMutex mutex;

    bool AToggle;

    bool FT;

    int FPS;

    CutRateDetectionThread *HCRateThread;

    Mat myFrame;
    Mat myAnalyticFrame;
    QVector<Mat> myMats;
    QVector<Mat> analytics1;
    QVector<Mat> analytics2;

    playBackThread *playBack;

    cv::Size nSz;


    
signals:
    void sendPix(QPixmap);
    void tFinished();
    void sendQImg(QImage);
    void sendMat(Mat);

    
public slots:
    void initVideoFrame();
    void findVideoStream();
    bool checkCodec();
    void initConverter();
    void setDstInts();
    void allocRGBPic();
    void sendPixVec();
    void sendPixHS(QPixmap pix);
    void sendMatsPB(Mat mat);

    
};

#endif // VIDEOTHREAD_H
