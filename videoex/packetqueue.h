#ifndef PACKETQUEUE_H
#define PACKETQUEUE_H

#include <QtCore>

#include "opencv2/core/core.hpp"

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

#include "qcondition.h"

class PacketQueue: public QObject
{
    Q_OBJECT

public:
    PacketQueue(QObject *parent = 0);

    AVPacketList *firstPkt, *lastPkt;
    int nb_packets;
    int size;
    QMutex mutex;
    QCondition *cond;


public slots:

    void setFirstPkt(AVPacketList *fPkt);
    void setLastPkt(AVPacketList *lPkt);
    void setNumPkts(int numberPkts);
    void setSize(int nSize);
    void lockMutex();
    void unlockMutex();


};

#endif // PACKETQUEUE_H
