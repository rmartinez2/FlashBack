#include "widget.h"
#include "ui_widget.h"
#include "mat2qimage.h"

using namespace cv;

#include <stdio.h>
#include <math.h>
#include <stdint.h>

AVCodecContext *setCodecCtxFields(AVCodecContext *stream, AVCodecContext *enc);

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    av_register_all();

    setUpGView();

    frameCounter = 0;
    toggle = true;
    mtoggle = false;
    stoggle = false;
    sliderToggle = false;
    notifs = false;

    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);


    const char* filename = "C:\\Users\\Rene\\Desktop\\Test.mp2";
    char* Path = "C:\\Users\\Rene\\Desktop\\L&O.mpg";

    AVDictionary *optionsDict = NULL;

    // "C:\\Users\\Rene\\Desktop\\silence.mp3" "C:\\Users\\Rene\\Desktop\\apollo_17_stroll.mpg" C:\\Users\\Rene\\Desktop\\AstroInMM.mpg
    //C:\\Users\\Rene\\Desktop\\AstroWithCuts.mpg

    if(avformat_open_input(&formCtx,"C:\\Users\\Rene\\Desktop\\L&O.mpg",NULL,NULL) != 0){
        qDebug() << "Did not open video";
    }




    if(avformat_find_stream_info(formCtx,NULL) < 0){
        qDebug() << "Couldn't find stream info";
    }
    //apollo_17_stroll.mpg

    //debugging info on video and stream
    av_dump_format(formCtx,0,"C:\\Users\\Rene\\Desktop\\L&O.mpg",0);

    //qDebug() << "Show me the money";


    ui->horizontalSlider->setRange(0,(int)formCtx->duration/30000);

   streamIndex = -1;
   svIndex = -1;
   streamIndex = av_find_best_stream(formCtx,AVMEDIA_TYPE_AUDIO,-1,-1,&pCodec,0);
   svIndex = av_find_best_stream(formCtx,AVMEDIA_TYPE_VIDEO,-1,-1,&vCodec,0);

    if(streamIndex > 0){

          audioStream = formCtx->streams[streamIndex];
          //pCodecCtx = audioStream->codec;
        //  pCodecCtx->codec = pCodec;


        //qDebug() << "Could not find audio stream in video";
    }

    if(svIndex > 0){
        videoStream = formCtx->streams[svIndex];
        vCodecCtx = videoStream->codec;
        vCodecCtx->codec = vCodec;
        QRect rect;

        rect.setHeight(vCodecCtx->height/2);
        rect.setWidth(vCodecCtx->width/2);

       // qDebug() << rect.height() << " " << rect.width();

        ui->graphicsView->setGeometry(rect);
        this->setGeometry(rect);


    }

    capTimer = new QTimer(this);
    cap.open(Path);
    double fps = cap.get(CV_CAP_PROP_FPS);
    capTimer->setInterval(1000.0/fps);


    HCRateThread = new CutRateDetectionThread();
    bsDetectThread = new BSDetectionThread();
    LDThread = new logoDetectionThread();

    main = new MainMenu();
    sMenu = new SideMenu();
    sVis = new SliderVisual();
    notes = new NotificationWidget();

    int menuY = 0;
    int menuX = 0;

    int menuW = vCodecCtx->width/2;
    int menuH = vCodecCtx->height/2;

    main->setXYWH(menuX,menuY,menuH,menuW);

    int menu2W = menuW/4;
    sMenu->setXYWH(menuX,menuY,menu2W,menuH);

    int menu3H = menuH/4;
    int menu3Y = 275;

    sVis->setXYWH(menuX,menu3Y,menuW,menu3H);

    notes->setXYWH(400,15,200,100);



    connect(HCRateThread,SIGNAL(highCuts(bool)),this,SLOT(recHighCuts(bool)));
    connect(bsDetectThread,SIGNAL(isBlack(bool)),this,SLOT(recBS(bool)));
    connect(LDThread,SIGNAL(noLogo(bool)),this,SLOT(recNoLogo(bool)));
    connect(capTimer,SIGNAL(timeout()),this,SLOT(drawMat2()));
    connect(this,SIGNAL(ldcrbufferFull()),this,SLOT(addFramesToThreads()));
    connect(this,SIGNAL(bsbufferFull()),this,SLOT(addFramesToBSThread()));


   matViewer = new cvMatViewer();

   ui->graphicsView->scene()->addWidget(matViewer);
   ui->graphicsView->scene()->addWidget(main);
   ui->graphicsView->scene()->addWidget(sMenu);
   ui->graphicsView->scene()->addWidget(sVis);
   ui->graphicsView->scene()->addWidget(notes);

   matViewer->show();

   if(cap.isOpened()){
    //usingImShow();
   capTimer->start();


   }


}

Widget::~Widget()
{

    cap.release();

    ldBuffer.clear();
    bsBuffer.clear();
    crBuffer.clear();

    LDThread->quit();
    bsDetectThread->quit();
    HCRateThread->quit();

    delete LDThread;
    delete bsDetectThread;
    delete HCRateThread;
    delete capTimer;

    delete sVis;
    delete main;
    delete matViewer;
    delete sMenu;
    delete notes;

    avformat_free_context(formCtx);

    delete ui;
}

void Widget::proFrameandUpdateGUI(){

    Mat frame1;
// player->play();
   // for(int i = 0; i < cap.get(CV_CAP_PROP_frame1_COUNT); i++){
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

void Widget::finishedPlaying(QAudio::State state){
    qDebug() << state;
         if (state == QAudio::IdleState) {
             output->stop();
             delete output;
         }
}

void Widget::encodeAndSave()
{

    while(av_read_frame(formCtx,&packet) == 0){


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

void Widget::drawMat(Mat mat)
{
    matViewer->showImg(mat);
    matViewer->show();
}

void Widget::drawMat2()
{
  //  if(cap.isOpened()){
        Mat frame;

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
    for(int i = 0; i < cap.get(CV_CAP_PROP_FRAME_COUNT); i++){
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

void Widget::recHighCuts(bool rec)
{
   // qDebug() << "Cut Rates";
}

void Widget::recBS(bool rec)
{
    //qDebug() << "Black Screen";
}

void Widget::recNoLogo(bool rec)
{
   // qDebug() << "No Logo";
}

void Widget::addFramesToThreads()
{

    if(!HCRateThread->isRunning()){
       // qDebug() << "HC Thread";
        HCRateThread->addFrames(crBuffer);
        HCRateThread->start();
    }



    if(!LDThread->isRunning()){
      LDThread->addFrames(ldBuffer);
      LDThread->start();
    }

    crBuffer.clear();
    ldBuffer.clear();
}

void Widget::addFramesToBSThread()
{

    if(!bsDetectThread->isRunning()){
        //qDebug() << "BS Thread";
        bsDetectThread->readInFrames(bsBuffer);
        bsDetectThread->start();
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




    }
}
