#include "playbackthread.h"

playBackThread::playBackThread(QVector<Mat> bufferedMats, QObject *parent) :
    QThread(parent)
{
    buffs = bufferedMats;
    bSDetectThread = new BSDetectionThread();
    HCRateThread = new CutRateDetectionThread();
    logoDectThread = new logoDetectionThread();
}

playBackThread::playBackThread(QObject *parent):
    QThread(parent)
{
    bSDetectThread = new BSDetectionThread();
    HCRateThread = new CutRateDetectionThread();
    logoDectThread = new logoDetectionThread();
}

playBackThread::playBackThread(char *fileName, QObject *parent):
    QThread(parent)
{
    filePath = fileName;
    bSDetectThread = new BSDetectionThread();
    HCRateThread = new CutRateDetectionThread();
    logoDectThread = new logoDetectionThread();

    connect(HCRateThread,SIGNAL(sendMats(Mat)),this,SLOT(fromCRate(Mat)));
    connect(bSDetectThread,SIGNAL(sendMyMat(Mat)),this,SLOT(fromBSDect(Mat)));
    connect(logoDectThread,SIGNAL(ldSendMat(Mat)),this,SLOT(fromldDect(Mat)));

    frameReady = new QTimer(this);
    connect(frameReady,SIGNAL(timeout()),this,SLOT(queryFrame()));

   // connect(HCRateThread,SIGNAL(highCuts(bool)),this,SLOT());
   // connect(bSDetectThread,SIGNAL(isBlack(bool)),this, SLOT());
   // connect(logoDectThread,SIGNAL(noLogo(bool)),this, SLOT());
}

void playBackThread::run()
{

   // qDebug() << buffs.size();
    bool toggle = true;
   // FPS = 1000* (1/FPS);

    cap.open(filePath);

    double fps = cap.get(CV_CAP_PROP_FPS);

    frameReady->setInterval(1000.00/fps);

    frameReady->start();

    //int codec = cap.get(CV_CAP_PROP_FOURCC);

   /* if(cap.isOpened()){

        Mat frame;



        for(int i = 0; i < cap.get(CV_CAP_PROP_FRAME_COUNT); i++){
            cap >> frame;


            if(!frame.empty()){
                emit sendMat(frame);

                //imshow("Video",frame);
                //waitKey(3);



                Mat reSiz;
                sz.height = frame.rows/8;
                sz.width = frame.cols/8;
                resize(frame,reSiz,sz);

                buffs.append(frame);
                bsAnalytics.append(reSiz);
                crAnalytics.append(reSiz);
                //ldAnalytics.append(reSiz);

            /*    if(i == 358){

                Point p1((int)900,(int)500);
                Point p2((int)1100,(int)700);
                Rect rec(p1,p2);
                Mat logo = frame(rec).clone();

                imwrite("C:\\Users\\Rene\\Desktop\\LogoTheMiddleLg.jpg",logo);
                //emit sendMat(logo);
               // qDebug() << " Frame number " << i;


            }*/

           // }

           /* if(analytics.size() % 100 == 0 && toggle == true && analytics.size() > 0){

                for(int j = 0; j < analytics.size(); j++){
                    /*Mat reSiz;
                    sz.height = buffs.at(j).rows/8;
                    sz.width = buffs.at(j).cols/8;
                    resize(buffs.at(j),reSiz,sz);
                    analytics.append(reSiz);
                    emit sendMat(analytics.at(j));

                    msleep(16.67);



                }

                toggle = false;

           // }else*/
           /* if(buffs.size() % 50 == 0 /*&& toggle == false*///)//{
               // qDebug() << analytics.size();
             /*   if(!bSDetectThread->isRunning()){
                bSDetectThread->readInFrames(bsAnalytics);
                bSDetectThread->start();
                }

                bsAnalytics.clear();
            }

            if(buffs.size() % 50 == 0){

                if(!HCRateThread->isRunning()){
                    HCRateThread->addFrames(crAnalytics);
                    HCRateThread->start();
                }
                crAnalytics.clear();
            }

            if(buffs.size() % 50 == 0){
                if(!logoDectThread->isRunning()){
                    logoDectThread->addFrames(buffs);
                    logoDectThread->start();

                }

               // ldAnalytics.clear();
            }



            //analytics.clear();
            msleep(16.67);
        }

    }





    /*for(int i = 0; i < buffs.size(); i++){
        //qDebug() << "here";
        emit sendMat(buffs.at(i));


        msleep(30);
    }*/

   // this->quit();
}


void playBackThread::readInBufs(QVector<Mat> buffered)
{
    buffs = buffered;

}

void playBackThread::setFPS(double FPS)
{
    this->FPS = FPS;
}

void playBackThread::fromCRate(Mat mat)
{
    emit sendMat(mat);
}

void playBackThread::fromBSDect(Mat mat)
{
    emit sendMat(mat);
}

void playBackThread::fromldDect(Mat mat)
{
    emit sendMat(mat);
}

void playBackThread::queryFrame()
{
    if(cap.isOpened()){

    Mat frame;

    qDebug() << "Time out";



    if(!frame.empty()){
        emit sendMat(frame);
    }

    }



}
