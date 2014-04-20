#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>

#include <QKeyEvent>
#include <QtCore>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QFile>
#include <QBuffer>
#include <QVector>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWaitCondition>
#include <QMutex>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>

#include "videothread.h"
#include "recordplayback.h"

#include "cvmatviewer.h"
#include "playbackthread.h"

#include "logodetectionthread.h"
#include "cutratedetectionthread.h"
#include "bsdetectionthread.h"

#include "mainmenu.h"
#include "sidemenu.h"
#include "slidervisual.h"
#include "notificationwidget.h"
#include "recordingoptions.h"
#include "recordings.h"

typedef struct dataHolder{
    uint8_t* data;
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


    QTimer *timer, *commercialTimer1, *commercialTimer2, *notesTimer;
    //cv::VideoCapture cap;


    FILE *f;


    QString recordFolder;

    AVFormatContext *formCtx1, *formCtx2;
    int i, streamIndex, svIndex, svIndex2;
    AVStream *audioStream, *videoStream, *videoStream2;
    AVCodecContext  *pCodecCtx, *eCodecCtx, *vCodecCtx, *vCodecCtx2;
    AVCodec *pCodec, *eCodec, *vCodec, *vCodec2;
    AVFrame *frame1, *vFrame;
    AVPacket packet;
    int frame1Finished;
    int numBytes;
    int buffersize1;
    uint *samples1;


    QBuffer buf;
    QBuffer vBuf;

    //QFile file;

    QByteArray data;
    QByteArray vData;

    VideoThread *vThread;


    QGraphicsScene *myScene;

    cvMatViewer *matViewer, *pipViewer;

    playBackThread *pbThread;

    VideoCapture cap;
    //VideoCapture cap2;

    VideoWriter writer1;
    VideoWriter writer2;

    QTimer *capTimer;
    QTimer *playBackTimer;

    QVector<Mat> bsBuffer;
    QVector<Mat> ldBuffer;
    QVector<Mat> crBuffer;
    QVector<Mat> frameStore;

    QList<Mat> mainBuffer;
    QList<Mat> secondaryBuffer;

    QVector<Mat> bsBuffer2;
    QVector<Mat> ldBuffer2;
    QVector<Mat> crBuffer2;

    //QList<dataHolder*> mainBuffer2;

    bool primary;

    bool ldPrimary;
    bool bsPrimary;
    bool crPrimary;

    bool ldSecondary;
    bool bsSecondary;
    bool crSecondary;

    bool primaryOnComm;
    bool secondaryOnComm;



    Size sz;

    int frameCounter;
    int frameCounter2;

    bool toggle;
    bool toggle2;
    bool toggle3;
    bool toggle4;

    int playBackIndex;


    CutRateDetectionThread *HCRateThread;
    logoDetectionThread *LDThread;
    BSDetectionThread *bsDetectThread;

    CutRateDetectionThread *HCRateThread2;
    logoDetectionThread *LDThread2;
    BSDetectionThread *bsDetectThread2;

    MainMenu *main;
    bool mtoggle;

    SideMenu *sMenu;
    bool stoggle;

    SliderVisual *sVis;
    bool sliderToggle;

    NotificationWidget *notes;
    bool notifs;


    RecordingOptions *recOps;
    bool recOptions;

    Recordings *recsWin;
    bool recsWindow;

    bool isPaused;

    QRect rect;
    QRect rect2;

    bool flashBackOn;

    bool isPIP;

    bool primeRec, secRec;

    int vid1FPS, vid2FPS;

    RecordPlayBack *recPBThread;




public slots:

   void proFrameandUpdateGUI();
   void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame);
//   void finishedPlaying(QAudio::State state);
   void encodeAndSave();

   void setImgLab(QPixmap pix);
   void setUpGView();
   void drawMat(uint8_t* data);
   void drawMat2(uint8_t *data);
   void drawMatCV();
   void fillBuffers(Mat mat, bool primaryCh);
   void usingImShow();

   void recHighCuts(bool rec);
   void recBS(bool rec);
   void recNoLogo(bool rec);

   void recHighCuts2(bool rec);
   void recBS2(bool rec);
   void recNoLogo2(bool rec);


   void addFramesToThreads(bool primaryCh);
   void addFramesToBSThread(bool primaryCh);

   void playBackFromBuf();
   void chkPrimaryForCB();
   void chkSecondaryForCB();

   void chkPrimaryCutRate();
   void chkSecondaryCutRate();

   void toggleFlashBack(bool on);

   void changeToPIP(bool inPIP);

   void recordPrimaryStream();
   void recordSecondaryStream();
   void cancelAllRecording();

   void setUpPB(const char* playBack);

    
private slots:
   void on_horizontalSlider_sliderMoved(int position);
   void setPaused(bool isPaused);
   void setPlay(bool isPlay);
   void closeNote();

protected:
   void keyPressEvent(QKeyEvent *a);

signals:
   void ldcrbufferFull(bool);
   void bsbufferFull(bool);
   void breakVthread(bool);



private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
