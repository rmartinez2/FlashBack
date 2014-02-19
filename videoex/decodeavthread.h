#ifndef DECODEAVTHREAD_H
#define DECODEAVTHREAD_H


#include <QtCore>
#include <QDebug>
#include <QThread>
#include <QByteArray>
#include <QBuffer>
#include <QPixmap>
#include <QRgb>
#include <QVector>
#include <QMutexLocker>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "bsdetectionthread.h"
#include "mat2qimage.h"

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

using namespace cv;

class decodeAVThread : public QThread
{
    Q_OBJECT
public:
    explicit decodeAVThread(AVFormatContext *formCtx, QObject *parent = 0);



    AVPacket packet;
    AVCodecContext *aCodecCtx = NULL;
    AVCodec *aCodec = NULL;
    QAudioFormat aFormat;
    QBuffer buf;
    AVFrame *aFrame;
    QAudioOutput *output;
    AVStream *audioStream;
    AVDictionary *aDict = NULL;

    QVector<int> buffData;
    QVector<Mat> myMats;
    QVector<QImage> myImgs;

    int aStream;
    uint *sampleSize;
    int bufferSize;


    AVFrame *vFrame, *vFrameRGB;
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
    QVector<QPixmap> pVec;
    QMutex mutex;

    BSDetectionThread *bsThread;

    int FPS;



    void run();
    
signals:
    void sendPix(QPixmap);
    void tFinished();
    void changeSlider(int);
    
public slots:

    void initVideoFrame();
    void findVideoStream();
    bool checkCodec();
    void initConverter();
    void setDstInts();
    void allocRGBPic();
    void sendPixVec();

    bool findAudioStream();
    void setAudioFormat();
    bool setAudioDeviceInfo();
    void finishedPlaying(QAudio::State state);
    void initAudioFrame();
    void getSampleSize();
    void getBufferSize();
    void fillAudioFrame();
    bool detectSilence();

    Mat QImagetoMat(QImage img);
    
};

#endif // DECODEAVTHREAD_H
