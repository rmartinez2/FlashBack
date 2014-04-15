#include "cvmatviewer.h"

/*bug #0 : delay to pop up menu's after paused for a period of time and menu is closed*/
/*bug #1 : openCV runs out of memory when storing too many frames in data structure, need to figure out how to fix*/

cvMatViewer::cvMatViewer(QWidget *parent) :
    QWidget(parent)
{
//    playing = true;


//    pausedTimer = new QTimer(this);
//    pausedTimer->setInterval(1000.0/59.94);

//    dataCpy = new pixelDataThread();


//    connect(pausedTimer,SIGNAL(timeout()),this,SLOT(pbAfterPause()));
}

cvMatViewer::~cvMatViewer()
{
//    holder.clear();
//    rewindHolder.clear();

//  //  delete rewindHolder;
//   // delete holder;
//    delete pausedTimer;

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


//    height = img.rows;
//    width = img.cols;
//    step = img.step;
//    channels = img.channels();

//    uchar* nData = (uchar*)malloc(height*width*img.channels()*sizeof(uchar));

//    for(int i = 0; i < height; i++){
//        for(int j = 0; j < step; j++){
//            nData[step * i + j ] = img.data[step * i + j ] / 3;
//            nData[step * i + j + 1] = img.data[step * i + j + 1] / 3;
//            nData[step * i + j + 2] = img.data[step * i + j + 2] / 3;
//        }
//    }




//    dataCpy->getArray(nData);
//    dataCpy->getData(img.data);
//    dataCpy->getWHS(height,width,step);

    //qDebug() << "in showImg "<< img.data;

  // dataCpy->start();


// if(playing && holder.isEmpty()){



//     holder.append(img);

//  // rewindHolder.append(img);

//    pausedTimer->start();

// }else if(playing == false ||  (playing == true && !holder.isEmpty())){

//     holder.append(img);

//   //  rewindHolder.append(img);

//     if(playing == false)
//         pausedTimer->stop();

//    }

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
