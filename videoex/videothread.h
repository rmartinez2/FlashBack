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

#include <QWaitCondition>
#include <QMutex>


using namespace cv;

class VideoThread : public QThread
{
    Q_OBJECT

public:
    explicit VideoThread(AVFormatContext *formCtx, AVFormatContext *formCtx2 , QObject *parent = 0);

    void run();

    ~VideoThread();

    AVPacket packet;
    AVFrame *vFrame, *vFrameRGB, *bgrFrame, *bgrFrame2, *vFrame2, *rFrame, *rBGRFrame;
    AVFormatContext *formCtx,*formatCtx2, *recFormCtx;
    QBuffer vBuf;
    QByteArray vData;
    AVStream *videostream1,*videostream2, *recVidStream;
    AVCodecContext *vCodecCtx1, *vCodecCtx2, *rCodecCtx;
    AVCodec *vCodec, *vCodec2, *rCodec;
    AVDictionary *cDict;
    SwsContext *imgConvertCtx, *imgConvertCtx2, *rImgCovtCtx;
    int vidStream,vidStream2, rStream;
    int destWidth, destHeight, destFmt, destWidth2, destHeight2, rDestWidth, rDestHeight;
    QVector <QImage>  qVec;
    QVector <QPixmap> pVec;


    QMutex mutex;
    QWaitCondition cond;


    bool AToggle;

    bool breaker;
    bool recPlayBack;

    int FPS;

    CutRateDetectionThread *HCRateThread;

    Mat myFrame;
    Mat myAnalyticFrame;
    QVector<Mat> myMats;
    QVector<Mat> analytics;

    playBackThread *playBack;

    cv::Size nSz;


    
signals:
    void sendPix(QPixmap);
    void tFinished();
    void sendQImg(QImage);
    void sendMat(Mat);
    void sendData1(uint8_t* data);
    void sendData2(uint8_t* data);
    
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
    void vThreadBreak(bool breaker);

    bool recordContext(AVFormatContext *recFrmCtx);
    void initRecordFrame();
    void findRecordStream();
    bool checkRecCodec();
    void initRecConverter();
    void setRecDstInts();
    void allocBGRPic();



    
};

#endif // VIDEOTHREAD_H
