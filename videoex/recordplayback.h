#ifndef RECORDPLAYBACK_H
#define RECORDPLAYBACK_H

#include <QThread>
#include <QtCore>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/cv.h"

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

class RecordPlayBack : public QThread
{
    Q_OBJECT
public:
    explicit RecordPlayBack(AVFormatContext *formCtx,QObject *parent = 0);
    ~RecordPlayBack();


    AVPacket packet;
    AVFrame *vFrame, *bgrFrame;
    AVFormatContext *formCtx;
    AVStream *videostream;
    AVCodecContext *vCodecCtx;
    AVCodec *vCodec;
    AVDictionary *cDict;
    SwsContext *imgConvertCtx;
    int vidStream;
    int destWidth, destHeight;

    void run();
    
signals:
    void sendFrameRecording(uint8_t* data);
    
public slots:
    void initVideoFrame();
    void findVideoStream();
    bool checkCodec();
    void initConverter();
    void setDstInts();
    void allocBGRPic();
    
};

#endif // RECORDPLAYBACK_H
