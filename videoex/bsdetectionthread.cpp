#include "bsdetectionthread.h"

BSDetectionThread::BSDetectionThread(QVector<QImage> images)
{
    //Pass in vector of QImages
    deter = images;
}

/*
 *BSDetectionThread or Black Screen Detection thread will convert
 *passed in QImages being tested, convert them to OpenCV Mats
 *than test each pixel in a Mat, if all the pixels are black we have a black screen
 *
 */

void BSDetectionThread::run()
{

    foreach (QImage img, deter) {
        Mat temp = QImage2Mat(img);


        for(int i = 0; i < temp.rows; i++){
            for(int j = 0; j < temp.cols; j++){

                if(temp.at<int>(i,j) > 0){
                    i = temp.rows;
                    j = temp.cols;

                }
                else if(i + 1 == temp.rows && j+1 == temp.rows){
                    qDebug() << "Black Frame";

                }
                else
                    continue;

            }
        }

    }

}
