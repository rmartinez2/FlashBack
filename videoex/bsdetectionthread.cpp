#include "bsdetectionthread.h"



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



                if((j != (int)900/8 && k != (int)500/8) || (j!= (int)1100/8 && k!= (int) 700/8)){
                cv::Vec3b bgrPixel = temp.at<cv::Vec3b>(j,k);
                 sum = sum + bgrPixel[0]
                           + bgrPixel[1]
                           + bgrPixel[2];
                }

            }
        }

        //qDebug() << "Sum in bs detect " << sum;

        if(sum < 1000000){

          //     qDebug() << "Black Screen Detected";
          //  emit sendMyMat(temp);
          //  char Check[50];
          //  std::cin >> Check;

            emit isBlack(true);


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


void BSDetectionThread::readInFrames(QList<Mat> mats)
{

    deter = mats;
    //sqDebug() << deter.size();

}
