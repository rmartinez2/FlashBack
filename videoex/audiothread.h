#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QtCore>
#include <QDebug>
#include <QThread>
#include <QByteArray>
#include <QBuffer>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
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


class AudioThread : public QThread
{
    Q_OBJECT
public:
    explicit AudioThread(AVFormatContext *formCtx,QObject *parent = 0);

    AVFormatContext *formCtx;
    AVPacket packet;
    AVCodecContext *aCodecCtx = NULL;
    AVCodec *aCodec = NULL;
    QAudioFormat aFormat;
    QBuffer buf;
    AVFrame *aFrame;
    QAudioOutput *output;
    QMutex mutex;
    AVStream *audioStream;
    AVDictionary *aDict = NULL;

    QVector<int> buffData;

    int aStream;
    uint *sampleSize;
    int bufferSize;


    void run();
    
signals:
    
public slots:
    bool findAudioStream();
    void setAudioFormat();
    bool setAudioDeviceInfo();
    void finishedPlaying(QAudio::State state);
    void initAudioFrame();
    void getSampleSize();
    void getBufferSize();
    void fillAudioFrame();
    bool detectSilence();

    
};

#endif // AUDIOTHREAD_H
