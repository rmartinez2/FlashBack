#include "widget.h"
#include "ui_widget.h"
#include "mat2qimage.h"

using namespace cv;

#include <stdio.h>
#include <math.h>
#include <stdint.h>

//use int index as a helper to go through the container
//play back from the actual buffer
//store all frames into one buffer than get copies of every 50 or 100 frames from buffer for algos
//store which frame number the algos are on within constructor
//when rewind is done...pause algos or some other adjustment
//play back frames from container with the timer and when the next frame needs to be requested just increment
//leave int index to be where the program current is being decoded
//use helper int to go backwards
//use a bool to determine if the helper int has caught up with the index
//ff would move through container the other way never passing or becoming greater than index
//helper would never be less than 0, if 0 is reached just begin play back from the beginning

AVCodecContext *setCodecCtxFields(AVCodecContext *stream, AVCodecContext *enc);

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{


    ui->setupUi(this);

    this->window()->setWindowTitle("FlashBack");

    av_register_all();



    setUpGView();

    frameCounter = 0;
    frameCounter2 = 0;

    toggle = true;
    toggle3 = true;

    mtoggle = false;
    stoggle = false;
    sliderToggle = false;
    notifs = false;
    isPaused = false;

    primary = true;

    isPIP = false;

     ldPrimary = false;
     bsPrimary = false;
     crPrimary = false;

     ldSecondary = false;
     bsSecondary = false;
     crSecondary = false;

     primaryOnComm = false;
     secondaryOnComm = false;



    playBackIndex = 0;

    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    timer = new QTimer(this);
    timer->setInterval(0.5);

    commercialTimer1 = new QTimer(this);
    commercialTimer1->setInterval(30000);

    commercialTimer2 = new QTimer(this);
    commercialTimer2->setInterval(30000);

    notesTimer = new QTimer(this);
    notesTimer->setSingleShot(true);
    notesTimer->setInterval(5000);

    formCtx1 = NULL;
    formCtx2 = NULL;

    const char* filename = "C:\\Users\\Rene\\Desktop\\Test.mp2";
    char* Path = "C:\\Users\\Rene\\Desktop\\L&O.mpg";
    char* Path2 = "C:\\Users\\Rene\\\Desktop\\EyeForEye.mpg";

    const char* logo1 = "C:\\Users\\Rene\\Desktop\\LogoTheMiddleLg.jpg";
    const char* logo2 = "C:\\Users\\Rene\\Desktop\\LogoCW.jpg";

    recordFolder = "C:/FlashBackRecording";
   // "C:\\Users\\Rene\\Desktop\\L&O.mpg"

    //Demo
    if(avformat_open_input(&formCtx1,"C:\\Users\\Rene\\Desktop\\L&O.mpg",NULL,NULL) != 0){
        qDebug() << "Did not open video";
    }


    if(avformat_find_stream_info(formCtx1,NULL) < 0){
        qDebug() << "Couldn't find stream info";
    }

    av_dump_format(formCtx1,0,"C:\\Users\\Rene\\Desktop\\L&O.mpg",0);
qDebug() << "here";
    if(avformat_open_input(&formCtx2,"C:\\Users\\Rene\\Desktop\\EyeForEye.mpg",NULL,NULL) != 0){
        qDebug() << "Did not open vide2";
    }


    if(avformat_find_stream_info(formCtx2,NULL) < 0){
        qDebug() << "Couldn't find stream info2";
    }

    av_dump_format(formCtx2,0,"C:\\Users\\Rene\\Desktop\\EyeForEye.mpg",0);


   streamIndex = -1;
   svIndex = -1;
  // streamIndex = av_find_best_stream(formCtx1,AVMEDIA_TYPE_AUDIO,-1,-1,&pCodec,0);
   svIndex = av_find_best_stream(formCtx1,AVMEDIA_TYPE_VIDEO,-1,-1,&vCodec,0);
   svIndex2 = av_find_best_stream(formCtx2,AVMEDIA_TYPE_VIDEO,-1,-1,&vCodec2,0);



//    if(streamIndex > 0){

//          audioStream = formCtx1->streams[streamIndex];
//          //pCodecCtx = audioStream->codec;
//        //  pCodecCtx->codec = pCodec;


//        //qDebug() << "Could not find audio stream in video";
//    }

    if(svIndex >= 0 && svIndex2 >= 0){

        videoStream = formCtx1->streams[svIndex];
        vCodecCtx = videoStream->codec;
        vCodecCtx->codec = vCodec;

        rect.setHeight(vCodecCtx->height/2);
        rect.setWidth(vCodecCtx->width/2);

        videoStream2 = formCtx2->streams[svIndex2];
        vCodecCtx2 = videoStream2->codec;
        vCodecCtx2->codec = vCodec;

        rect2.setHeight(vCodecCtx2->height);
        rect2.setWidth(vCodecCtx2->width);

       AVRational rat1 = videoStream->r_frame_rate;
       AVRational rat2  = videoStream2->r_frame_rate;

       vid1FPS = rat1.num/rat1.den;
       vid2FPS = rat2.num/rat2.den;

       // qDebug() << rect.height() << " " << rect.width();
        QRect rect3;
        rect3.setHeight(rect.height() + 7);
        rect3.setWidth(rect.width() + 7);


        ui->graphicsView->setGeometry(rect3);
        this->setGeometry(rect3);


    }else{
        qDebug() << "Could not find video streams" << svIndex << " " << svIndex2;
    }

    vThread = new VideoThread(formCtx1,formCtx2);


    capTimer = new QTimer(this);
    //cap.open(Path2);
    //cap2.open(Path2);
//    double fps = cap.get(CV_CAP_PROP_FPS);
   // qDebug() << "FPS " << fps;
//    capTimer->setInterval(1);

    playBackTimer = new QTimer(this);
    playBackTimer->setInterval(0.5);


    HCRateThread = new CutRateDetectionThread();
    bsDetectThread = new BSDetectionThread();
    LDThread = new logoDetectionThread();

    HCRateThread2 = new CutRateDetectionThread();
    bsDetectThread2 = new BSDetectionThread();
    LDThread2 = new logoDetectionThread();

    Point p11((int)900, (int)500);
    Point p12((int)1100, (int)700);
    Point p21((int)825, (int)575);
    Point p22((int)1015, (int)650);


    LDThread->setLogoLoc(p11,p12);
    LDThread->setLogo(logo1);

    LDThread2->setLogoLoc(p21,p22);
    LDThread2->setLogo(logo2);

    bsDetectThread->readInPointVals(p11,p12);
    bsDetectThread2->readInPointVals(p21,p22);


   // main = new MainMenu();
    sMenu = new SideMenu();
    sVis = new SliderVisual();
    notes = new NotificationWidget();
    recOps = new RecordingOptions();
    recsWin = new Recordings();

    int menuY = 0;
    int menuX = 0;

    int menuW = vCodecCtx->width/2;
    int menuH = vCodecCtx->height/2;

    recsWin->setXYWH(menuX,menuY,menuW,menuH);
    recsWin->setRecordDir(recordFolder);

    int menu2W = menuW/4;
    sMenu->setXYWH(menuX,menuY,menu2W,menuH);

    int menu3H = menuH/4;
    int menu3Y = 275;

    sVis->setXYWH(menuX,menu3Y,menuW,menu3H);

    recOps->setXYWH(menuX,menuY,menuW,menu3H);

    notes->setXYWH(400,15,200,100);

    connect(sMenu,SIGNAL(toggleFB(bool)),this,SLOT(toggleFlashBack(bool)));

    connect(HCRateThread,SIGNAL(highCuts(bool)),this,SLOT(recHighCuts(bool)));
    connect(bsDetectThread,SIGNAL(isBlack(bool)),this,SLOT(recBS(bool)));
    connect(LDThread,SIGNAL(noLogo(bool)),this,SLOT(recNoLogo(bool)));

    connect(HCRateThread2,SIGNAL(highCuts(bool)),this,SLOT(recHighCuts2(bool)));
    connect(bsDetectThread2,SIGNAL(isBlack(bool)),this,SLOT(recBS2(bool)));
    connect(LDThread2,SIGNAL(noLogo(bool)),this,SLOT(recNoLogo2(bool)));

//    connect(capTimer,SIGNAL(timeout()),this,SLOT(drawMat2()));
    connect(playBackTimer,SIGNAL(timeout()),this,SLOT(playBackFromBuf()));

    connect(this,SIGNAL(ldcrbufferFull(bool)),this,SLOT(addFramesToThreads(bool)));
    connect(this,SIGNAL(bsbufferFull(bool)),this,SLOT(addFramesToBSThread(bool)));


    connect(vThread,SIGNAL(sendData1(byte*)),this,SLOT(drawMat(byte*)));
    connect(vThread,SIGNAL(sendData2(byte*)),this,SLOT(drawMat2(byte*)));
    connect(this,SIGNAL(breakVthread(bool)),vThread,SLOT(vThreadBreak(bool)));

    connect(timer,SIGNAL(timeout()),this,SLOT(chkPrimaryForCB()));
    connect(timer,SIGNAL(timeout()),this,SLOT(chkSecondaryForCB()));

    connect(commercialTimer1,SIGNAL(timeout()),this,SLOT(chkPrimaryCutRate()));
    connect(commercialTimer2,SIGNAL(timeout()),this,SLOT(chkSecondaryCutRate()));
    connect(notesTimer,SIGNAL(timeout()),this,SLOT(closeNote()));


    connect(sMenu,SIGNAL(showRecordings(bool)),recsWin,SLOT(setVisible(bool)));
    connect(sMenu,SIGNAL(showRecordings(bool)),this,SLOT(changeToPIP(bool)));
    connect(recsWin,SIGNAL(outofPIP(bool)),this,SLOT(changeToPIP(bool)));
    connect(recsWin,SIGNAL(sendForPB(const char*)),this,SLOT(setUpPB(const char*)));

    connect(sMenu,SIGNAL(showRecordingOpts(bool)),recOps,SLOT(setVisible(bool)));
    connect(recOps,SIGNAL(primaryRecord(bool)),this,SLOT(recordPrimaryStream()));
    connect(recOps,SIGNAL(secondaryRecord(bool)),this,SLOT(recordSecondaryStream()));
    connect(recOps,SIGNAL(cancelRecording()),this,SLOT(cancelAllRecording()));

    connect(capTimer,SIGNAL(timeout()),this,SLOT(drawMatCV()));




   matViewer = new cvMatViewer();
   pipViewer = new cvMatViewer();
   recsWin->setPIP(pipViewer);

   ui->graphicsView->scene()->addWidget(matViewer);
  // ui->graphicsView->scene()->addWidget(main);
   ui->graphicsView->scene()->addWidget(sMenu);
   ui->graphicsView->scene()->addWidget(sVis);
   ui->graphicsView->scene()->addWidget(notes);
   ui->graphicsView->scene()->addWidget(recsWin);
   ui->graphicsView->scene()->addWidget(recOps);


   connect(sVis,SIGNAL(sendPause(bool)),this,SLOT(setPaused(bool)));
   connect(sVis,SIGNAL(sendPlay(bool)),this,SLOT(setPlay(bool)));

   notes->setTopText("Test Top");
   notes->setBottomText("Test Bottom");
   notes->updateTime();

   matViewer->show();

   flashBackOn = true;

   vThread->start();
   playBackTimer->start();
   timer->start();



//   if(cap.isOpened()){
//   // usingImShow();
//   //capTimer->start();
//  // playBackTimer->start();
//   }


}

Widget::~Widget()
{

    //cap.release();

   // qDebug() << mainBuffer.size();
    ldBuffer.clear();
    bsBuffer.clear();
    crBuffer.clear();
    mainBuffer.clear();

    LDThread->quit();
    bsDetectThread->quit();
    HCRateThread->quit();

    delete LDThread;
    delete bsDetectThread;
    delete HCRateThread;
    delete capTimer;

    delete sVis;
   // delete main;
    delete matViewer;
    delete sMenu;
    delete notes;

    delete recPBThread;
    delete vThread;

    cap.release();

    //writer1.release();
    //writer2.release();

    avformat_free_context(formCtx1);
    avformat_free_context(formCtx2);

    delete ui;
}

void Widget::proFrameandUpdateGUI(){

    Mat frame1;
// player->play();
   // for(int i = 0; i < cap.get(CV_CAP_PROP_frame1_COUNT); i++){
       // cap >> frame1;

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

void Widget::SaveFrame(AVFrame *pFrame, int width, int height, int iFrame){
  FILE *pFile;
  char szFilename[32];
  int  y;

  // Open file
  sprintf(szFilename, "C:\\Users\\Rene\\Desktop\\frame%d.ppm", iFrame);
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;

  // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", width, height);

  // Write pixel data
  for(y=0; y<height; y++)
    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

  // Close file
  fclose(pFile);
}

AVCodecContext* setCodecCtxFields(AVCodecContext *stream, AVCodecContext *enc){
    enc->bit_rate = stream->bit_rate;
    enc->sample_fmt = stream->sample_fmt;
    enc->sample_rate    = stream->sample_rate;
    enc->channel_layout = stream->channel_layout;
    enc->channels       = av_get_channel_layout_nb_channels(enc->channel_layout);

    return enc;
}

//void Widget::finishedPlaying(QAudio::State state){
//    qDebug() << state;
//         if (state == QAudio::IdleState) {
//             output->stop();
//             delete output;
//         }
//}

void Widget::encodeAndSave()
{

    while(av_read_frame(formCtx1,&packet) == 0){


        if(packet.stream_index == audioStream->index){
            frame1Finished = 0;

              avcodec_decode_audio4(pCodecCtx,frame1,&frame1Finished,&packet);


              if(frame1Finished){
              int chk3 = avcodec_encode_audio2(eCodecCtx,&packet,frame1,&i);


               if(chk3 < 0){
                    qDebug() << stderr << " Could not encode audio frames";
                }

                if(i){

                    if((int)packet.data >= 0 && (int)packet.data <= 0x55c620){

                    }else{
                        fwrite(packet.data,1,packet.size,f);

                    }
                }
              }
            }
         av_free_packet(&packet);
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

void Widget::drawMat(uint8_t *data)
{

    Mat myFrame(rect.height()*2,rect.width()*2,CV_8UC3, data);

    fillBuffers(myFrame,true);
    mainBuffer.append(myFrame);


    //matViewer->showImg(myFrame);
      //= (byte*)malloc(sizeof(data)*sizeof(byte));
   // dataHolder* hold = (dataHolder*)malloc(sizeof(dataHolder));
   // memcpy(&hold->data,&data,sizeof(data));
   // memcpy(&hold,&data,sizeof(data));
   // qDebug() << sizeof(byte);
   // mainBuffer2.append(hold);
   // matViewer->showImg(myFrame);



}

void Widget::drawMat2(uint8_t *data)
{

    Mat myFrame(rect.height()*2,rect.width()*2,CV_8UC3, data);
    fillBuffers(myFrame,false);
    secondaryBuffer.append(myFrame);

}

void Widget::drawMatCV()
{
   Mat frame;

   cap >> frame;



   if(!isPIP){
       Mat temp;

       Size reSiz;
       reSiz.height = rect.height();
       reSiz.width = rect.width();

       cv::resize(frame,temp,reSiz);
       matViewer->showImg(temp);

   }else{
       Mat temp;

       Size reSiz;
       reSiz.height = 720/8;
       reSiz.width = 1280/8;

       cv::resize(frame,temp,reSiz);
       pipViewer->showImg(temp);
   }

}

void Widget::fillBuffers(Mat mat, bool primaryCh)
{
    if(primaryCh){
    if(toggle){

        Mat reSiz;
        sz.height = mat.rows/8;
        sz.width = mat.cols/8;
        cv::resize(mat,reSiz,sz);

    ldBuffer.append(mat);
    crBuffer.append(reSiz);
    }

    if(toggle2){
        Mat reSiz;
        sz.height = mat.rows/8;
        sz.width = mat.cols/8;
        cv::resize(mat,reSiz,sz);

        bsBuffer.append(reSiz);

    }



    frameCounter++;

    if(frameCounter % 50 == 0){

        if(toggle == true){
            emit ldcrbufferFull(true);

        }


        toggle ^= true;
        //qDebug() << frameCounter << " " << toggle;
        //qDebug() << bsBuffer.size();
    }

    if(frameCounter % 100 == 0){
        if(toggle2 == true){
            emit bsbufferFull(true);
        }

        toggle2 ^= true;
    }

    }else{

        if(toggle3){

            Mat reSiz;
            sz.height = rect.height();
            sz.width = rect.width();
            cv::resize(mat,reSiz,sz);




        ldBuffer2.append(mat);
        crBuffer2.append(reSiz);
        }

        if(toggle4){
            Mat reSiz;
            sz.height = mat.rows/8;
            sz.width = mat.cols/8;
            cv::resize(mat,reSiz,sz);

            bsBuffer2.append(reSiz);

        }



        frameCounter2++;

        if(frameCounter2 % 50 == 0){

            if(toggle3 == true){
                emit ldcrbufferFull(false);

            }


            toggle3 ^= true;
            //qDebug() << frameCounter << " " << toggle;
            //qDebug() << bsBuffer.size();
        }

        if(frameCounter2 % 100 == 0){
            if(toggle4 == true){
                emit bsbufferFull(false);
            }

            toggle4 ^= true;
        }

    }





}

void Widget::usingImShow()
{
    for(int i = 0; i < cap.get(CV_CAP_PROP_FRAME_COUNT); i++){
        Mat frame;

        cap >> frame;

        if(!frame.empty()){

            Size reSiz;
            reSiz.height = rect.height()*2;
            reSiz.width = rect.width()*2;

            Mat temp;
            cv::resize(frame,temp,reSiz);

            Point p21((int)825, (int)575);
            Point p22((int)1015, (int)650);
            Rect rec(p21,p22);

            Mat temp2 = temp(rec).clone();

            //qDebug() << temp2.cols << " " << temp2.rows << " " << temp.cols << " " << temp.rows;

            imshow("Video",temp);
            imshow("Logo",temp2);

            if(i == 350)
            imwrite("C:\\Users\\Rene\\Desktop\\LogoCW.jpg",temp2);

           // imshow("Logo Frm File", logo);

//            if(i == 350){
//                Point p1((int)415,(int)290);
//                Point p2((int)505,(int)320);
//                Rect rec(p1,p2);
//                Mat logo = temp(rec).clone();

//                imwrite("C:\\Users\\Rene\\Desktop\\LogoCW.jpg",logo);

//                //imshow("Logo",logo);
//               // waitKey(0);
//            }
        }

        waitKey(1);
    }
}

void Widget::recHighCuts(bool rec)
{

    crPrimary = rec;

    if(primaryOnComm && !crPrimary)
        chkPrimaryForCB();

   //  qDebug() << "Cut Rates1 " << crPrimary;
}

void Widget::recBS(bool rec)
{
//    if(rec)
//    qDebug() << "Black Screen";
//    else
//        qDebug() << "No Black Screen";
    bsPrimary = rec;
}

void Widget::recNoLogo(bool rec)
{/*
    if(rec)
        qDebug() << "No Logo1";
    else
        qDebug() << "Logo 1";*/

    ldPrimary = rec;
}

void Widget::recHighCuts2(bool rec)
{
   // qDebug() << "Cut Rates 2";

    crSecondary = rec;

    if(secondaryOnComm && !crSecondary)
        chkSecondaryForCB();

    //qDebug() << "Cut Rates 2 " << crSecondary;

}

void Widget::recBS2(bool rec)
{
//    if(rec)
//    qDebug() << "Black Screen2";
//    else
//        qDebug() << "No Black Screen2";

    bsSecondary = rec;
}

void Widget::recNoLogo2(bool rec)
{
//    if(rec)
//        qDebug() << "No Logo 2";
//    else
//        qDebug() << "Logo 2";

    ldSecondary = rec;
}

void Widget::addFramesToThreads(bool primaryCh)
{

    if(primaryCh){
    if(!HCRateThread->isRunning()){
       // qDebug() << "HC Thread";
        HCRateThread->addFrames(crBuffer);
        if(flashBackOn)
        HCRateThread->start();
    }



    if(!LDThread->isRunning()){
      LDThread->addFrames(ldBuffer);
      if(flashBackOn)
      LDThread->start();
    }

    crBuffer.clear();
    ldBuffer.clear();

    }else{
        if(!HCRateThread2->isRunning()){
           // qDebug() << "HC Thread";
            HCRateThread2->addFrames(crBuffer2);
            if(flashBackOn)
            HCRateThread2->start();
        }



        if(!LDThread2->isRunning()){
          LDThread2->addFrames(ldBuffer2);
          if(flashBackOn)
          LDThread2->start();
        }

        crBuffer2.clear();
        ldBuffer2.clear();


    }
}

void Widget::addFramesToBSThread(bool primaryCh)
{
    if(primaryCh){
    if(!bsDetectThread->isRunning()){
        //qDebug() << "BS Thread";
        bsDetectThread->readInFrames(bsBuffer);
        if(flashBackOn)
        bsDetectThread->start();
    }

     bsBuffer.clear();
    }else{
        if(!bsDetectThread2->isRunning()){
            //qDebug() << "BS Thread";
            bsDetectThread2->readInFrames(bsBuffer2);
            if(flashBackOn)
            bsDetectThread2->start();
        }

         bsBuffer2.clear();
    }
}

void Widget::playBackFromBuf()
{


    //if(isPaused == false){

 if(!isPIP){
    if(primary){
        if(!mainBuffer.isEmpty()){

            Size reSiz;
            reSiz.height = rect.height();
            reSiz.width = rect.width();


            Mat temp = mainBuffer.at(0);



            Mat temp2;
            cv::resize(temp,temp2,reSiz);
            matViewer->showImg(temp2);

            if(primeRec && !primaryOnComm){
                writer1 << temp2;
            }


        }
        }else{
            if(!secondaryBuffer.isEmpty()){
                Size reSiz;
                reSiz.height = rect.height();
                reSiz.width = rect.width();



                Mat temp = secondaryBuffer.at(0);
                Mat temp2;
                cv::resize(temp,temp2,reSiz);
                matViewer->showImg(temp2);

                if(secRec && !secondaryOnComm){
                    writer2 << temp2;
                }


            }

        }
}
    else{
        if(primary){
            if(!mainBuffer.isEmpty()){

                Size reSiz;
                reSiz.height = 720/8;
                reSiz.width = 1280/8;


                Mat temp = mainBuffer.at(0);

                if(primeRec && !primaryOnComm){
                    writer1 << temp;
                }


                Mat temp2;
                cv::resize(temp,temp2,reSiz);
                pipViewer->showImg(temp2);


        }
        }else{
            if(!secondaryBuffer.isEmpty()){
                Size reSiz;
                reSiz.height = 720/8;
                reSiz.width = 1280/8;



                Mat temp = secondaryBuffer.at(0);

                if(secRec && !secondaryOnComm){
                    writer2 << temp;
                }

                Mat temp2;
                cv::resize(temp,temp2,reSiz);
                pipViewer->showImg(temp2);


            }
    }
}


}

void Widget::chkPrimaryForCB()
{
    if(primaryOnComm == false){

    if(ldPrimary && bsPrimary){
      //  qDebug() << "Primary on Commercial";
        primaryOnComm = true;

        if(!secondaryOnComm){
            notes->setTopText("Switching to Secondary");
            notes->setBottomText("Primary on Commercial Break at");
            notes->updateTime();
            notes->setVisible(true);
            notesTimer->start();
            //Switching to secondary channel
        }


        }

    }else{

        if(!ldPrimary && !bsPrimary && !crPrimary){
          //  qDebug() << "Primary on programming";
            primaryOnComm = false;

            if(!primary){

                notes->setTopText("Switching to Primary");
                notes->setBottomText("Primary on Commercial Break at");
                notes->updateTime();
                notes->setVisible(true);
                notesTimer->start();
                //notify that primary stream is off commercial and switch
            }


        }


    }


}

void Widget::chkSecondaryForCB()
{

    if(secondaryOnComm == false){
     if(ldSecondary && bsSecondary){
        qDebug() << "Secondary on Commercial";
       secondaryOnComm = true;

       if(primary){
           notes->setTopText("Secondary Update");
           notes->setBottomText("Secondary on Commercial Break at");
           notes->updateTime();
           notes->setVisible(true);
           notesTimer->start();
           //notify secondary channel is on commercial
       }




        }

    }else{
        if(!ldSecondary && !bsSecondary && !crSecondary){
       // qDebug() << "Secondary on programming";
        secondaryOnComm = false;

        if(primary){
            notes->setTopText("Secondary Update");
            notes->setBottomText("Secondary is off Commercial Break at");
            notes->updateTime();
            notes->setVisible(true);
            notesTimer->start();
            //notify secondary channel is off commercial
        }

    }
    }



}

void Widget::chkPrimaryCutRate()
{
    //qDebug() << "Cut Rate Timer1 Time out";
    if(!crPrimary){
       // qDebug() << "Regular Cut Rate 1";
        chkPrimaryForCB();
    }
}

void Widget::chkSecondaryCutRate()
{
     //qDebug() << "Cut Rate Timer2 Time out";
    if(!crSecondary){
       // qDebug() << "Regular Cut Rate 2";
        chkSecondaryForCB();
    }
}

void Widget::toggleFlashBack(bool on)
{
    if(on){
        HCRateThread->start();
        HCRateThread2->start();

        bsDetectThread->start();
        bsDetectThread2->start();

        LDThread->start();
        LDThread2->start();

      //  qDebug() << "FB on";

        flashBackOn = true;

        timer->start();

    }else{
        HCRateThread->quit();
        HCRateThread2->quit();

        bsDetectThread->quit();
        bsDetectThread2->quit();

        LDThread->quit();
        LDThread2->quit();
       // qDebug() << "FB off";
        //also add a graphic that Flashback is off
        flashBackOn = false;
        timer->stop();
    }
}

void Widget::changeToPIP(bool inPIP)
{
    this->isPIP = inPIP;
    //qDebug() << inPIP;

}

void Widget::recordPrimaryStream()
{
    Size sz;
    sz.height = rect.height();
    sz.width = rect.width();

    writer1.open(recordFolder.toStdString() + "/PrimaryStream.avi",CV_FOURCC('M','P','E','G'),vid1FPS,sz,true);

    if(!writer1.isOpened()){
        qDebug() << "Failed to open writer";
    }else{
        //qDebug() << "Opened";
        primeRec = true;
    }


}

void Widget::recordSecondaryStream()
{
    Size sz;
    sz.height = rect.height();
    sz.width = rect.width();

   // string secPath = recordFolder.toStdString();
    writer2.open(recordFolder.toStdString() + "/SecondaryStream.avi",CV_FOURCC('M','P','E','G'),vid2FPS,sz,true);

    if(!writer2.isOpened()){
        qDebug() << "Failed to open writer";
    }else{
       // qDebug() << "Opened";
        secRec = true;
    }
}




void Widget::on_horizontalSlider_sliderMoved(int position)
{
    ui->horizontalSlider->setValue(position);
}

void Widget::setPaused(bool isPaused)
{
    this->isPaused = isPaused;
    playBackTimer->stop();

}

void Widget::setPlay(bool isPlay)
{
    this->isPaused = isPlay;
    playBackTimer->start();
}

void Widget::closeNote()
{
    notes->setVisible(false);

    if(primaryOnComm)
        primary = false;

    if(!primaryOnComm)
        primary = true;

}

void Widget::keyPressEvent(QKeyEvent *a)
{
    //Do a switch here with a char to determine which key was pressed...
    //May be a long switch statement.

    int keyPressed = a->key();

    qDebug() << keyPressed;

    switch(keyPressed){

    case 65:
        mtoggle ^= true;
        break;

    case 66:
        stoggle ^= true;
        sMenu->setVisible(stoggle);
        break;

    case 67:
        sliderToggle ^= true;
        sVis->setVisible(sliderToggle);
        break;

    case 68:
        notifs ^= true;
        notes->setVisible(notifs);
        break;

    case 69:
        primary ^=true;
        break;

    case 96:
        isPIP = false;

        recOps->setVisible(false);
        recsWin->setVisible(false);
        break;



    }
}


void Widget::cancelAllRecording()
{
    primeRec = false;
    secRec = false;

    //writer1.release();
    //writer2.release();


}

void Widget::setUpPB(const char *playBack)
{
   // AVFormatContext *rPBFormCtx = NULL;

    qDebug() << playBack;

//    if(avformat_open_input(&rPBFormCtx,playBack,NULL,NULL) != 0){
//        qDebug() << "Did not open video";
//    }


//    if(avformat_find_stream_info(rPBFormCtx,NULL) < 0){
//        qDebug() << "Couldn't find stream info";
//    }

//    av_dump_format(rPBFormCtx,0,playBack,0);


//    recPBThread = new RecordPlayBack(rPBFormCtx);

//    connect(recPBThread,SIGNAL(sendFrameRecording(byte*)),this,SLOT(drawMat(byte*)));

//    if(cap.isOpened())
//    cap.release();


    HCRateThread->quit();
    HCRateThread2->quit();
    LDThread->quit();
    LDThread2->quit();
    bsDetectThread->quit();
    bsDetectThread2->quit();

   // vThread->exit();
    emit breakVthread(true);
    while(vThread->isRunning()){

        qDebug() << "Vthread is running";
    }
   // delete vThread;

    timer->stop();
    playBackTimer->stop();
    primary = true;
    flashBackOn = false;

    if(!vThread->isRunning()){
       // emit breakVthread(false);

        if(cap.isOpened()){
            capTimer->stop();
            cap.release();
        }

        cap.open(playBack);
        if(cap.isOpened()){
        capTimer->setInterval(1000/30);
        capTimer->start();
        }else{
            qDebug() << "Could not open Recording";
            emit breakVthread(false);
            vThread->start();
            playBackTimer->start();
        }
        //drawMatCV();
    }

}
