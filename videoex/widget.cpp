#include "widget.h"
#include "ui_widget.h"
#include "mat2qimage.h"

using namespace cv;

#include <stdio.h>
#include <math.h>
#include <stdint.h>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    av_register_all();


   // system("azap -a 0 -r -c /home/marlon/seniorDesignLinaro/tunerDev/channels.conf WKCF-DT &");
    //system("azap -a 1 -r -c /home/marlon/seniorDesignLinaro/tunerDev/channels.conf WKMG-DT &");

    //writer.open("x.mpg", CV_FOURCC('M', 'P', 'E', 'G'), 30, Size(640, 320),true );
    setUpGView();

    frameCounter = 0;
    toggle = true;
    toggle2 = true;
    mtoggle = false;
    stoggle = false;


    if(avformat_open_input(&formCtx,"/dev/dvb/adapter0/dvr0",NULL,NULL) != 0){
        qDebug() << "Did not open video";
    }



    if(avformat_find_stream_info(formCtx,NULL) < 0){
        qDebug() << "Couldn't find stream info";
    }

    //apollo_17_stroll.mpg

    //debugging info on video and stream
   //av_dump_format(formCtx,0,"/dev/dvb/adapter0/dvr0",0);

    //qDebug() << "Show me the money";


    //ui->horizontalSlider->setRange(0,(int)formCtx->duration/30000);

   streamIndex = -1;
   svIndex = -1;
//   streamIndex = av_find_best_stream(formCtx,AVMEDIA_TYPE_AUDIO,-1,-1,&pCodec,0);
//   svIndex = av_find_best_stream(formCtx,AVMEDIA_TYPE_VIDEO,-1,-1,&vCodec,0);

//    if(streamIndex > 0){

//          audioStream = formCtx->streams[streamIndex];
//          //pCodecCtx = audioStream->codec;
//        //  pCodecCtx->codec = pCodec;


//        //qDebug() << "Could not find audio stream in video";
//    }

    //if(svIndex > 0){
//        videoStream = formCtx->streams[svIndex];
//        vCodecCtx = videoStream->codec;
//        vCodecCtx->codec = vCodec;
        QRect rect;

        rect.setHeight(360);
        rect.setWidth(640);

        ui->graphicsView->setGeometry(rect);
        this->setGeometry(rect);


    //}

    capTimer = new QTimer(this);
  // mCvCapturePtr = cvCaptureFromCAM(1);

    //cap.open("dev/video1");
    //cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
   // cap.set(CV_CAP_PROP_FRAME_HEIGHT,320);
    //double fps = cap.get(CV_CAP_PROP_FPS);
    double fps = 30;
    capTimer->setInterval(1000/fps);


    HCRateThread = new CutRateDetectionThread();
    bsDetectThread = new BSDetectionThread();
    LDThread = new logoDetectionThread();

    main = new MainMenu();
    sMenu = new SideMenu();

    int menuY = 0;
    int menuX = 0;

    int menuW = 360;
    int menuH = 640;

    main->setXYWH(menuX,menuY,menuH,menuW);


    connect(HCRateThread,SIGNAL(highCuts(bool)),this,SLOT(recHighCuts()));
    connect(bsDetectThread,SIGNAL(isBlack(bool)),this,SLOT(recBS(bool)));
    connect(LDThread,SIGNAL(noLogo(bool)),this,SLOT(recNoLogo(bool)));
    connect(capTimer,SIGNAL(timeout()),this,SLOT(drawMat2()));
    connect(this,SIGNAL(ldcrbufferFull()),this,SLOT(addFramesToThreads()));
    connect(this,SIGNAL(bsbufferFull()),this,SLOT(addFramesToBSThread()));


   matViewer = new cvMatViewer();

   ui->graphicsView->scene()->addWidget(matViewer);
   ui->graphicsView->scene()->addWidget(main);

   matViewer->show();


    VideoThread *vThread = new VideoThread(formCtx,NULL);

    qRegisterMetaType<Mat>("Mat");
    connect(vThread,SIGNAL(sendMat(Mat)),matViewer,SLOT(showImg(Mat)));
    connect(vThread,SIGNAL(sendMat(Mat)),this,SLOT(fillBuffers(Mat)));



    vThread->start();


   if(cap.isOpened()){
    //usingImShow();
   capTimer->start();

  }


}

Widget::~Widget()
{
    //cap.release();


    ldBuffer.clear();
    bsBuffer.clear();
    crBuffer.clear();

    LDThread->quit();
    bsDetectThread->quit();
    HCRateThread->quit();

    delete LDThread;
    delete bsDetectThread;
    delete HCRateThread;

    delete matViewer;
    avformat_free_context(formCtx);

    delete ui;

}

void Widget::proFrameandUpdateGUI(){

    Mat frame1;
// player->play();
   // for(int i = 0; i < cap.get(CV_CAP_PROP_frame1_COUNT); i++){
    //mCvCapturePtr >> frame1;
      cap >> frame1;

        if(!frame1.empty()){
            int height = frame1.rows;
            int width = frame1.cols;
            QImage image = Mat2QImage(frame1);

            QPixmap imMap = QPixmap::fromImage(image);
            QRect rect;
            rect.setHeight(height);
            rect.setWidth(width);


            //ui->imageLab->setGeometry(rect);
           // ui->imageLab->setPixmap(imMap);
            //imshow("Testing Video",frame1);
           //waitKey(cap.get(CV_CAP_PROP_FPS));
        //}
    }
}

//void Widget::SaveFrame(AVFrame *pFrame, int width, int height, int iFrame){
//  FILE *pFile;
//  char szFilename[32];
//  int  y;

//  // Open file
//  sprintf(szFilename, "C:\\Users\\Rene\\Desktop\\frame%d.ppm", iFrame);
//  pFile=fopen(szFilename, "wb");
//  if(pFile==NULL)
//    return;

//  // Write header
//  fprintf(pFile, "P6\n%d %d\n255\n", width, height);

//  // Write pixel data
//  for(y=0; y<height; y++)
//    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

//  // Close file
//  fclose(pFile);
//}

//AVCodecContext* setCodecCtxFields(AVCodecContext *stream, AVCodecContext *enc){
//    enc->bit_rate = stream->bit_rate;
//    enc->sample_fmt = stream->sample_fmt;
//    enc->sample_rate    = stream->sample_rate;
//    enc->channel_layout = stream->channel_layout;
//    enc->channels       = av_get_channel_layout_nb_channels(enc->channel_layout);

//    return enc;
//}

void Widget::finishedPlaying(QAudio::State state){
    qDebug() << state;
         if (state == QAudio::IdleState) {
             output->stop();
             delete output;
         }
}





void Widget::setImgLab(QPixmap pix)
{
    //qDebug() << "Got pixmap";
    //ui->imageLab->setPixmap(pix);

    myScene->addPixmap(pix);



}

void Widget::setUpGView()
{
    myScene = new QGraphicsScene();

    ui->graphicsView->setScene(myScene);
}

void Widget::drawMat(Mat mat)
{
    matViewer->showImg(mat);
    matViewer->show();
}

void Widget::drawMat2()
{
  //  if(cap.isOpened()){


    //cvGrabFrame(mCvCapturePtr); // capture a frame
    //IplImage* img = cvRetrieveFrame(mCvCapturePtr); // retrieve the captured frame

    //IplImage* img = cvQueryFrame(mCvCapturePtr);
    //    Mat frame1(img);
    Mat frame;
       // frame1 >> frame;
       // Mat frame;
        cap >> frame;

        if(!frame.empty()){

            frameStore.append(frame);
            fillBuffers(frame);
            Size reSiz;
            reSiz.height = frame.rows/2;
            reSiz.width = frame.cols/2;

            Mat temp;
            cv::resize(frame,temp,reSiz);
            matViewer->showImg(temp);

        }


   // }
}

void Widget::fillBuffers(Mat mat)
{

    Mat copyMat = mat.clone();

    if(toggle){
    //qDebug() << "Got to Fill Buffers";
        //Mat reSiz;
       // sz.height = mat.rows/8;
       // sz.width = mat.cols/8;
       // cv::resize(mat,reSiz,sz);

    ldBuffer.append(copyMat);
    crBuffer.append(copyMat);
    }

    if(toggle2){
       // Mat reSiz;
       // sz.height = mat.rows/8;
       // sz.width = mat.cols/8;
       // cv::resize(mat,reSiz,sz);

         bsBuffer.append(copyMat);

    }



    frameCounter++;

    if(frameCounter % 50 == 0){

        if(toggle == true){
            emit ldcrbufferFull();

       }


        toggle ^= true;
        //qDebug() << frameCounter << " " << toggle;
        //qDebug() << bsBuffer.size();
    }

    if(frameCounter % 100 == 0){
        if(toggle2 == true){
            emit bsbufferFull();
        }

        toggle2 ^= true;
    }





}

void Widget::usingImShow()
{
    for(int i = 0; i < 30; i++){
        Mat frame;

        cap >> frame;

        if(!frame.empty()){

            Size reSiz;
            reSiz.height = frame.rows/2;
            reSiz.width = frame.cols/2;

            Mat temp;
            cv::resize(frame,temp,reSiz);


            imshow("Video",temp);
        }

        waitKey(5);
    }
}

void Widget::recHighCuts()
{
   qDebug() << "Cut Rates";
}

void Widget::recBS(bool rec)
{
    qDebug() << "Black Screen";
}

void Widget::recNoLogo(bool rec)
{
   // qDebug() << "No Logo";
}

void Widget::addFramesToThreads()
{

    if(!HCRateThread->isRunning()){
       //qDebug() << "HC Thread";
       HCRateThread->addFrames(crBuffer);
       HCRateThread->start();
    }



//    if(!LDThread->isRunning()){
//       LDThread->addFrames(ldBuffer);
//       LDThread->start();
//    }

    crBuffer.clear();
    //ldBuffer.clear();
}

void Widget::addFramesToBSThread()
{

    if(!bsDetectThread->isRunning()){
        //qDebug() << "BS Thread";
        bsDetectThread->readInFrames(bsBuffer);
      // bsDetectThread->start();
    }

     bsBuffer.clear();
}




void Widget::on_horizontalSlider_sliderMoved(int position)
{
    ui->horizontalSlider->setValue(position);
}

void Widget::keyPressEvent(QKeyEvent *a)
{
    //Do a switch here with a char to determine which key was pressed...
    //May be a long switch statement.

    int keyPressed = a->key();



    switch(keyPressed){

    case 65:
        mtoggle ^= true;
        main->setVisible(mtoggle);
        break;

    case 66:
        stoggle ^= true;

        break;





    }
}
