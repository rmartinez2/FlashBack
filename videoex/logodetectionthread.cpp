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
    scale = 1;
    delta = 0;
    ddepth = CV_16S;

    logoMat = imread("C:\\Users\\Rene\\Desktop\\LogoTheMiddleLg.jpg");

    GaussianBlur(logoMat,logoGray,Size(3,3),0,0,BORDER_DEFAULT);
    cvtColor(logoGray,logoGray,CV_BGR2GRAY);

    Sobel(logoGray,gradX,ddepth,1,0,3,scale,delta,BORDER_DEFAULT);
    convertScaleAbs(gradX,absGradX);

    Sobel(logoGray,gradY,ddepth,0,1,3,scale,delta,BORDER_DEFAULT);
    convertScaleAbs(gradY,absGradY);

    addWeighted(absGradX,0.5,absGradY,0.5,0,gradLogo);

}

void logoDetectionThread::run()
{

    Point p1((int)900, (int)500);
    Point p2((int)1100, (int)700);

    Rect box(p1,p2);

    int sum = 0;

    /*IplImage newLogo = gradLogo;
    cvNormalize(&newLogo,&newLogo,0,255,CV_MINMAX);

    Mat seep1(&newLogo);*/


    for(int i = 0; i < myMats.size(); i++){
        Mat temp = myMats.at(i);
        Mat cpy = temp(box).clone();


        GaussianBlur(cpy,cpy,Size(3,3), 0, 0, BORDER_DEFAULT);
        cvtColor(cpy,cpy,CV_BGR2GRAY);

        Sobel(cpy,gradX,ddepth,1,0,3,scale,delta,BORDER_DEFAULT);
        convertScaleAbs(gradX,absGradX);

        Sobel(cpy,gradY,ddepth,0,1,3,scale,delta,BORDER_DEFAULT);
        convertScaleAbs(gradY,absGradY);

        addWeighted(absGradX,0.5,absGradY,0.5,0,gradTemp);



      /*  IplImage newImg = gradTemp;

        cvNormalize(&newImg,&newImg,0,255,CV_MINMAX);

        Mat seep(&newImg);

         temp = seep1 - seep;*/

    temp = gradLogo - gradTemp;

        for(int j = 0; j < temp.rows; j++){
            for(int k = 0; k < temp.cols; k++){
                sum = sum + temp.at<uchar>(j,k);
            }
        }

        //qDebug() << sum;

        if(sum > 200000){

            emit noLogo(true);
          //qDebug() << "Logo has disappeared";
        }else{
            emit noLogo(false);
        }


        sum = 0;

       // emit ldSendMat(temp);
       // msleep(16.67);
    }
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


