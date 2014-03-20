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

void logoDetectionThread::run()
{
}
