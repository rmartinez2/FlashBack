#include "bsdetectionthread.h"

BSDetectionThread::BSDetectionThread(QVector<QImage> images)
{
    deter = images;
}

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
