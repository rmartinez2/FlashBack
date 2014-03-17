#include "cutratedetectionthread.h"

//Store stdDev2 than get the stdDev2 of th next 5 frames, compare to current stdDev and StdDev2 that's stored
//decide if on commercial break from that realization

//Avoid counting cuts in which the stdDev of past frames and current frames aren't too far of a difference
//1) Support vector model may be a solution to creating a threshold for the each new data point in the model.
//2) Percentage diff from current stdDev

CutRateDetectionThread::CutRateDetectionThread(QVector<QImage> frameSamples, int index, QObject *parent) :
    QThread(parent)
{
    mySamples = frameSamples;
    gIndex = index;

}

CutRateDetectionThread::CutRateDetectionThread(QVector<QImage> frameSamples, QObject *parent) :
    QThread(parent)
{
    mySamples = frameSamples;
}

CutRateDetectionThread::CutRateDetectionThread(QVector<QPixmap> pixMapSamples, QObject *parent) :
    QThread(parent)
{
    myPixmaps = pixMapSamples;
}

void CutRateDetectionThread::run()
{
   int sum, mean;
   long long stdDev = 0;
   int step = 5;
   QVector<int> sumPix;
  //for(int i = gIndex; i < gIndex + 5; i++){
      // Mat conv = QImage2Mat(mySamples.at(i));
      // Mat2String(i,conv);
       //QImage2String(mySamples.at(i));
       // myMats.append(conv);
    //   getPixelData(mySamples.at(i));
   // }

   bool samples = true;

   //Obtain the frame samples from pixMap Vector
   for(int i = 0; i < myPixmaps.size(); i++){

       //Samples is a boolean that toggles
       if(samples){

        QPixmap pix = myPixmaps.at(i);
        sum = getPixelData(pix);

        //qDebug() << "Sum of Pix " << sum;


        sendPixMap(pix);
        char Check[50];
        std::cin >> Check;
        sumPix.append(sum);


       }

        if(i % step == 0 && i != 0){

            if(samples == true){

                long long hold = 0;

                for(int i = 0; i < sumPix.size(); i++){
                    mean += sumPix.at(i);
                }

                mean /= step;
                qDebug() << "Mean " << mean;

                if(stdDev == 0){
                    for(int cnt = 0; cnt < sumPix.size(); cnt++){
                        long long temp = 0, temp2 = 0;
                        temp += (sumPix.at(cnt) - mean);

                        temp2 = temp*temp;
                        hold += temp2;
                    }

                  stdDev = (long long)sqrt(hold/(sumPix.size()-1));
                  qDebug() << "StdDev of First 5 Frames " << stdDev;

                }else{


                    long long stdDev2 = 0;
                    long long hold = 0;

                    for(int j = 0; j < sumPix.size(); j++){
                        long long temp = 0;
                        long long temp2 = 0;
                        temp += (sumPix.at(j) - mean);
                        temp2 = temp*temp;
                        hold += temp2;
                   }

                    stdDev2 = (long long) sqrt(hold/(step-1));

                    if(stdDev2s.isEmpty())
                        stdDev2s.append(stdDev2);

                    else{

                        long long diff1 = stdDev - stdDev2;
                        long long diff2 = stdDev2s.at(0) - stdDev2;

                        if(diff1 < 0)
                            diff1 = -diff1;
                        if(diff2 < 0)
                            diff2 = -diff2;

                         qDebug() << "StdDev Old: " << stdDev;

                         if(diff2 < diff1){

                            switch(plusOrMinus(stdDev2s.at(0),stdDev2,diff2)){

                            case PER1:
                                qDebug() << "Cut into past frames Deviation";
                                stdDev = stdDev2s.at(0);
                                break;

                            case PER2:
                                qDebug() << "Cut into current Deviation";
                                stdDev = stdDev2;
                                break;

                            case -1:
                                qDebug() << "No Change in Deviation";
                               break;

                            }

                            /*if(plusOrMinus(stdDev2s.at(0),stdDev2 ,diff2)){

                             if(diff2 < stdDev2s.at(0)){
                              //  qDebug() << "Here 1";
                                stdDev = stdDev2s.at(0);
                            }else if(diff2 < stdDev2){
                               // qDebug() << "Here 2";
                                stdDev = stdDev2;
                            }
                          }*/

                        }

                        qDebug() << "StdDev New: " << stdDev
                                << " StdDev Current: " << stdDev2 << "StdDev Past Frames: " << stdDev2s.at(0);
                        qDebug() << "Diff1 " << diff1 << "Diff2 " << diff2;

                        stdDev2s.clear();
                        stdDev2s.append(stdDev2);
                    }

                    //qDebug() << "Standard Dev2 " << stdDev2;
                }

                mean = 0;
                sumPix.clear();
            }

            samples ^= true;
        }

   }




}

QString CutRateDetectionThread::QImage2String(QImage img)
{

    QString temp = "dupe";

    //img = img.convertToFormat(QImage::Format_ARGB32);

    for(int i = 0; i < img.height(); i++){
      //  for(int j = 0; j < img.height(); j++){
            //QRgb intensity = QRgb(img.pixel(i,j));
            QRgb *rowData = (QRgb*)img.scanLine(i);
            QRgb tempPix = *rowData;
            qDebug() << qGreen(tempPix) << " " << (uchar*)rowData;

       // }
    }

    //temp.append(QString::number(img.height(),10) + " " + QString::number(img.width(),10));


    return temp;
}

QString CutRateDetectionThread::Mat2String(int frameNumb,Mat mat)
{


QImage dest(mat.cols, mat.rows, QImage::Format_ARGB32);

    qDebug() << "Frame Number: " << frameNumb;
    int rows = mat.rows;
    int cols = mat.cols;
    //int r,g,b;

   // if(mat.isContinuous()){
       // qDebug() << "is continuous";
        //cols = rows*cols;
       //rows = 1;
   //}

    if(mat.depth() == CV_8U)
        qDebug() << "yes";


    if(mat.channels() == 3){

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            cv::Vec3b bgrPixel = mat.at<cv::Vec3b>(i,j);

            qDebug() << "blue: " << 255 - bgrPixel[0]
                     << " green: " << 255 - bgrPixel[1]
                     << " red: " << 255 - bgrPixel[2];
        //dest.setPixel(j,i,qRgb(bgrPixel[2],bgrPixel[1],bgrPixel[0]));
        }
      }
    }

   /* unsigned char b,g,r;

    for(int i = 0; i < rows; i++){
    unsigned char *data = mat.ptr(i);

    for(int j = 0; j < cols; j++){
        b = *data++;
        g = *data++;
        r = *data++;
        dest.setPixel(j,i,qRgb(r,g,b));
    }
   }*/

  //  QPixmap *pix = new QPixmap(mat.cols,mat.rows);
 //   pix->fromImage(dest,Qt::ColorOnly);

  //  emit sendPixMap(*pix);

 //   msleep(32);

    QString poop = " ";

    return poop;

}

void CutRateDetectionThread::getPixelData(QImage img)
{

   // QByteArray myBytes((char*)img.bits(),img.byteCount());

   // for(int i = 0; i < 8; i++){
     //   qDebug() << "Byte: " << img.bits()[i];
   // }

    QPixmap *pix = new QPixmap(img.width(),img.height());
    pix->fromImage(img,Qt::ColorOnly);

    emit sendPixMap(*pix);



    msleep(32);


}

int CutRateDetectionThread::getPixelData(QPixmap pix)
{
    int sum = 0;

    QImage img = pix.toImage();
    img = img.convertToFormat(QImage::Format_RGB888);

   //  QByteArray myBytes((char*)img.bits(),img.byteCount());

    for(int i = 0; i < (img.width()*img.height()*3); i++){
         sum += (int)img.bits()[i];
        //qDebug() << "Byte: " << img.bits()[i];
     }

    /* for(int i = 0; i < img.height(); i++){
         for(int j = 0; j < img.width(); j++){
             sum += img.pixel(i,j);
         }
     }*/

    return (int)sum;

}



/*int CutRateDetectionThread::plusOrMinus(long long stdDevNew, long long stdDevCur, long long diff)
{
    //qDebug() << stdDevCur << " " << stdDevNew << " " << diff;

    int DN = (int) stdDevNew;
    int DC = (int) stdDevCur;
    int D = (int) diff;

    double per1 = ((double)D/(double)DN);
    double per2 = ((double)D/(double)DC);

    qDebug() << "Percent Diff/DevNew " << per1 << " Per2 " << per2;

    if((per1 > 0.5 && per1 < 1.0) && (per2 > 1.0 && per2 < 0.5))
        return PER1;

      if((per1 < 0.5 && per1 > 1.0) && (per2 < 1.0 && per2 > 0.5))
          return PER2;



   // double digDiff = diff % 10;

    //double checks = (double)(diff/stdDevNew)*100;
  //  qDebug() << digDiff;


}*/

int CutRateDetectionThread::plusOrMinus(long long stdDevNew, long long stdDevCur, long long diff)
{

    int DN = (int) stdDevNew;
    int DC = (int) stdDevCur;
    int D = (int) diff;

    double per1 = ((double)D/(double)DN);
    double per2 = ((double)D/(double)DC);

    qDebug() << "Percent Diff/DevNew " << per1 << " Per2 " << per2;

   /* if(per1 > 0.5 && per2 > 0.5)
        return false;*/

    if(per1 < 0.5 && per2 < 0.5){
        if(per1 < per2){
            return PER2;
       } else
            return PER1;

    }else if(per1 > 0.5 && per2 < 0.5){
        return PER2;

    }else if(per1 < 0.5 && per2 > 0.5){
        return PER1;

    }else{
        return -1;
    }


   // return true;


}
