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


class VideoThread : public QThread
{
    Q_OBJECT

public:
    explicit VideoThread(AVFormatContext *formCtx, QObject *parent = 0);

    void run();

    AVPacket packet;
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

    int FPS;



    
signals:
    void sendPix(QPixmap);
    void tFinished();
    
public slots:
    void initVideoFrame();
    void findVideoStream();
    bool checkCodec();
    void initConverter();
    void setDstInts();
    void allocRGBPic();
    void sendPixVec();
    
};

#endif // VIDEOTHREAD_H
