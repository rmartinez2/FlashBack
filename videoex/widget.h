#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>
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

#include <QWaitCondition>
#include <QMutex>


#include "videothread.h"
#include "audiothread.h"
#include "decodeavthread.h"

#include "frameglwidget.h"
#include "cvmatviewer.h"
#include "playbackthread.h"

#include "logodetectionthread.h"
#include "cutratedetectionthread.h"
#include "bsdetectionthread.h"

#include "mainmenu.h"
#include "sidemenu.h"
#include "slidervisual.h"
#include "notificationwidget.h"

typedef struct dataHolder{
    byte* data;
}dataHolder;


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
    //cv::VideoCapture cap;
    QAudioOutput *output;

    FILE *f;




    AVFormatContext *formCtx1 = NULL, *formCtx2 = NULL;
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



    QBuffer buf;
    QBuffer vBuf;

    //QFile file;

    QByteArray data;
    QByteArray vData;

    QVideoWidget *vWid;

    VideoThread *vThread;
    AudioThread *aThread;
    decodeAVThread *avThread;

    QGraphicsScene *myScene;

    cvMatViewer *matViewer;
    playBackThread *pbThread;

    VideoCapture cap;
    VideoCapture cap2;

    QTimer *capTimer;
    QTimer *playBackTimer;

    QVector<Mat> bsBuffer;
    QVector<Mat> ldBuffer;
    QVector<Mat> crBuffer;
    QVector<Mat> frameStore;

    QList<Mat> mainBuffer;
    QList<Mat> secondaryBuffer;
    //QList<dataHolder*> mainBuffer2;

    bool primary;



    Size sz;

    int frameCounter;
    bool toggle;
    bool toggle2;

    int playBackIndex;


    CutRateDetectionThread *HCRateThread;
    logoDetectionThread *LDThread;
    BSDetectionThread *bsDetectThread;

    MainMenu *main;
    bool mtoggle;

    SideMenu *sMenu;
    bool stoggle;

    SliderVisual *sVis;
    bool sliderToggle;

    NotificationWidget *notes;
    bool notifs;

    bool isPaused;

    QRect rect;




public slots:

   void proFrameandUpdateGUI();
   void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame);
   void finishedPlaying(QAudio::State state);
   void encodeAndSave();

   void setImgLab(QPixmap pix);
   void setUpGView();
   void drawMat(byte* data);
   void drawMat2(byte *data);
   void fillBuffers(Mat mat);
   void usingImShow();
   void recHighCuts(bool rec);
   void recBS(bool rec);
   void recNoLogo(bool rec);

   void addFramesToThreads();
   void addFramesToBSThread();

   void playBackFromBuf();


    
private slots:
   void on_horizontalSlider_sliderMoved(int position);
   void setPaused(bool isPaused);
   void setPlay(bool isPlay);

protected:
   void keyPressEvent(QKeyEvent *a);

signals:
   void ldcrbufferFull();
   void bsbufferFull();



private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
