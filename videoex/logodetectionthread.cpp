#include "logodetectionthread.h"

//First step is to build data base from channels I can obtain OTA
//Get logo locations on screen for the given channel than normalize it
//Using the location found obtain the image in the logo location of streaming video for every 5th frame
//Normalize this image than compare it to the normalized logo of the channel
//Need to set a threshold for when i can consider a channel off comercial when images are compared

logoDetectionThread::logoDetectionThread(QVector<QImage> samples, QObject *parent) :
    QThread(parent)
{
}

logoDetectionThread::logoDetectionThread(QObject *parent):
    QThread(parent)
{
    logoMat = imread("C:\\Users\\Rene\\Desktop\\LogoTheMiddleLg.jpg");
    cvtColor(logoMat,logoGray,CV_BGR2GRAY);

}

void logoDetectionThread::run()
{

   // emit ldSendMat(logoMat);






    this->quit();
}


void logoDetectionThread::addFrames(QVector<Mat> frames)
{
    myMats = frames;
}

void logoDetectionThread::setLogoLoc(int x1, int y1, int x2, int y2)
{
    this->x1 = x1;
    this->y1 = y1;

    this->x2 = x2;
    this->y2 = y2;
}


