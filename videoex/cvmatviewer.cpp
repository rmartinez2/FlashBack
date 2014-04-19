#include "cvmatviewer.h"

/*bug #0 : delay to pop up menu's after paused for a period of time and menu is closed*/
/*bug #1 : openCV runs out of memory when storing too many frames in data structure, need to figure out how to fix*/

cvMatViewer::cvMatViewer( QWidget *parent) :
    QWidget(parent)
{

}

cvMatViewer::~cvMatViewer()
{

}

QSize cvMatViewer::sizeHint() const
{
    return qimg.size();
}

QSize cvMatViewer::minimumSizeHint() const
{
    return qimg.size();
}


void cvMatViewer::showImg(const Mat &img)
{


    switch(img.type()){

    case CV_8UC1:
        cvtColor(img, mat,CV_GRAY2RGB);
        break;

    case CV_8UC3:
        cvtColor(img, mat,CV_BGR2RGB);
        break;

    }


   qimg = QImage(mat.data, mat.cols, mat.rows, mat.cols*3, QImage::Format_RGB888);
   this->setFixedSize(img.cols,img.rows);

   repaint();


}

void cvMatViewer::isPaused()
{
  // playing = false;
  // pausedTimer->stop();

}

void cvMatViewer::isPlaying()
{
 //  playing = true;
 //  pausedTimer->start();


}

void cvMatViewer::pbAfterPause()
{
    if(!holder.isEmpty()){


       // uchar* temp = holder->takeFirst();
        //uchar* nData = (uchar*)malloc(height*width*channels*sizeof(uchar));



        // qDebug() << "in pbAfterPause "<< temp;

         Mat img = holder.takeFirst();

//         if(temp != NULL){
//             //qDebug() << "here";
//             img = Mat(height,width,CV_8UC3,temp,step);
//         }else{
//             img = Mat(height,width,CV_8UC3,Scalar::all(0));
//        }


      // Mat img(height,width,CV_8UC3,temp,step);





    }
}

void cvMatViewer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), qimg);
    painter.end();
}
