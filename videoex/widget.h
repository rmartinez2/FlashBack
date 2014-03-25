#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtMultimedia>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVideoWidget>
#include <QGraphicsVideoItem>
#include <QDebug>
#include <QFile>

#include <QAudioOutput>
#include <QIODevice>
#include <QMultimedia>
#include <QBuffer>
#include <QtCore>

#include <QMediaRecorder>
#include <QAudioEncoderSettings>
#include <QVideoEncoderSettings>

#include <QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>

#include <QVideoWidget>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "packetqueue.h"
#include "videopicture.h"
#include "videostate.h"
#include "qcondition.h"
#include "videothread.h"
#include "audiothread.h"
#include "decodeavthread.h"

#include "frameglwidget.h"
#include "cvmatviewer.h"
#include "playbackthread.h"


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


namespace Ui {
class Widget;
}

using namespace cv;
using namespace std;

class Widget : public QWidget
{
    Q_OBJECT
    //Q_DECLARE_METATYPE(Mat)

    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QTimer *timer;
    cv::VideoCapture cap;
    QAudioOutput *output;


    VideoState *globalVideoState;

    AVFormatContext *formCtx = NULL;
    int i, streamIndex, svIndex;
    AVStream *audioStream, *videoStream;
    AVCodecContext  *pCodecCtx = NULL, *eCodecCtx = NULL, *vCodecCtx = NULL;
    AVCodec *pCodec = NULL, *eCodec = NULL, *vCodec = NULL;
    AVFrame *frame1 = NULL, *vFrame = NULL;
    AVPacket packet;
    int frame1Finished;
    int numBytes;
    int buffersize1;
    uint *samples1 = NULL;
    UINT8 *medBuffer = NULL;

    QMutex mutex;

    QBuffer buf;
    QBuffer vBuf;

    //QFile file;

    QByteArray data;
    QByteArray vData;

    QVideoWidget *vWid;

    FILE *f;

    VideoThread *vThread;
    AudioThread *aThread;
    decodeAVThread *avThread;

    QGraphicsScene *myScene;

    cvMatViewer *matViewer;
    playBackThread *pbThread;



public slots:

   void proFrameandUpdateGUI();
   void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame);
   void finishedPlaying(QAudio::State state);
   void encodeAndSave();
   void packetQueueInit(PacketQueue *q);
   int packetQueuePut(PacketQueue *q, AVPacket *packet);
   int packetQueueGet(PacketQueue *q, AVPacket *packet, int block);
   double getAudioClock(VideoState *is);
   void setImgLab(QPixmap pix);
   void setUpGView();
   void drawMat(Mat mat);

    
private slots:
   void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
