#ifndef VIDEOSTATE_H
#define VIDEOSTATE_H

#include <QtCore>
#include <QThread>

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

#include "packetqueue.h"


class VideoState
{

public:
    VideoState();

    AVFormatContext *pFormatCtx;
    int audiostream, videostream;
    double audio_clock;

    AVStream *audioSt;
    AVStream *videoSt;

    PacketQueue audioQ;
    PacketQueue videoQ;

    AVPacket audioPkt;
    AVPacket videoPkt;

    QThread parseThread;
    QThread videoThread;

    QMutex picQMutex;
    QCondition *picQCond;

    const char *filename;

    unsigned int audio_buf_size;
    unsigned int audio_buf_index;

    AVFrame *audioPktdata;


    int quit;


public slots:


};

#endif // VIDEOSTATE_H
