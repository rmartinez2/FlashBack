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

//#include "packetqueue.h"
//#include "videopicture.h"
//#include "videostate.h"
//#include "qcondition.h"
#include "videothread.h"
//#include "audiothread.h"
//#include "decodeavthread.h"

//#include "frameglwidget.h"
#include "cvmatviewer.h"
#include "playbackthread.h"

#include "logodetectionthread.h"
#include "cutratedetectionthread.h"
#include "bsdetectionthread.h"

#include "mainmenu.h"
#include "sidemenu.h"

#include <QList>

extern "C"
{
    #include "libavcodec/avcodec.h"
   #include "libavformat/avformat.h"
   #include "libavutil/imgutils.h"
    #include "libavformat/avio.h"
    #include "libavutil/time.h"
    #include "libswscale/swscale.h"
   // #include "libavresample/avresample.h"
   #include "libavutil/opt.h"

}
//#include "cap_ffmpeg_impl.hpp"


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


    //VideoState *globalVideoState;
    AVFormatContext *formCtx = NULL;

    int i, streamIndex, svIndex;

    int frame1Finished;
    int numBytes;
    int buffersize1;
    uint *samples1 = NULL;


    QMutex mutex;

    QBuffer buf;
    QBuffer vBuf;

    //QFile file;

    QByteArray data;
    QByteArray vData;

    QVideoWidget *vWid;

    FILE *f;

    //VideoThread *vThread;
    //AudioThread *aThread;
    //decodeAVThread *avThread;

    QGraphicsScene *myScene;

    cvMatViewer *matViewer;
    playBackThread *pbThread;

    VideoCapture cap;
/**************************************************/
    CvCapture* mCvCapturePtr;
    VideoWriter writer;


    QTimer *capTimer;

    QList<Mat> bsBuffer;
    QVector<Mat> ldBuffer;
    QVector<Mat> crBuffer;
    QVector<Mat> frameStore;

    Size sz;

    int frameCounter;
    bool toggle;
    bool toggle2;


    CutRateDetectionThread *HCRateThread;
    logoDetectionThread *LDThread;
    BSDetectionThread *bsDetectThread;

    MainMenu *main;
    bool mtoggle;

    SideMenu *sMenu;
    bool stoggle;




public slots:

   void proFrameandUpdateGUI();
   //void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame);
   void finishedPlaying(QAudio::State state);
  // void encodeAndSave();
   //void packetQueueInit(PacketQueue *q);
   //int packetQueuePut(PacketQueue *q, AVPacket *packet);
   //int packetQueueGet(PacketQueue *q, AVPacket *packet, int block);
   //double getAudioClock(VideoState *is);
   void setImgLab(QPixmap pix);
   void setUpGView();
   void drawMat(Mat mat);
   void drawMat2();
   void fillBuffers(Mat mat);
   void usingImShow();
   void recHighCuts();
   void recBS(bool rec);
   void recNoLogo(bool rec);

   void addFramesToThreads();
   void addFramesToBSThread();


    
private slots:
   void on_horizontalSlider_sliderMoved(int position);

protected:
   void keyPressEvent(QKeyEvent *a);

signals:
   void ldcrbufferFull();
   void bsbufferFull();


private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
