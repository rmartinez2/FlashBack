#include "bsdetectionthread.h"

BSDetectionThread::BSDetectionThread(QVector<Mat> images, QObject *parent):
    QThread(parent)
{
    deter = images;
}

BSDetectionThread::BSDetectionThread(QObject *parent):
    QThread(parent)
{
}

void BSDetectionThread::run()
{

    //qDebug() << "BS Thread Started";

    for(int i = 0; i < deter.size(); i++){

        Mat temp = deter.at(i);
        long long sum = 0;
        for(int j = 0; j < temp.rows; j++){
            for(int k = 0; k < temp.cols; k++){



                if((j != (int)p1.x/8 && k != (int)p1.y/8) || (j!= (int)p2.x/8 && k!= (int) p2.y/8)){
                cv::Vec3b bgrPixel = temp.at<cv::Vec3b>(j,k);
                 sum = sum + bgrPixel[0]
                           + bgrPixel[1]
                           + bgrPixel[2];
                }

            }
        }

            //qDebug() << sum;

        if(sum < 200000){

              qDebug() << "Black Screen Detected";
          //  emit sendMyMat(temp);
          //  char Check[50];
          //  std::cin >> Check;

            emit isBlack(true);


        }else{
            emit isBlack(false);
        }

      //  qDebug() << "Black Screen Sum: " << (long long) sum;

      //  emit sendMyMat(temp);
        //char Check[50];
        //std::cin >> Check;

      //  msleep(16.67);




    }




     deter.clear();
     this->quit();

}


void BSDetectionThread::readInFrames(QVector<Mat> mats)
{

    deter = mats;
    //sqDebug() << deter.size();

}

void BSDetectionThread::readInPointVals(Point p1, Point p2)
{
    this->p1 = p1;
    this->p2 = p2;
}
