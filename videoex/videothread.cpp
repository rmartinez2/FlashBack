#include "videothread.h"

VideoThread::VideoThread(AVFormatContext *formCtx, AVFormatContext *formCtx2, QObject *parent) :
    QThread(parent)
{
    this->formCtx = formCtx;
    this->formatCtx2 = formCtx2;
    //this->mutex = mutex;

    av_init_packet(&packet);

    //av_lockmgr_register();




    findVideoStream();

    FT = false;
    AToggle = true;

    if(checkCodec() == true){
    setDstInts();
    initVideoFrame();
    initConverter();
    allocRGBPic();
    }else{
        qDebug() << "Failed to open codec";
    }


}

void VideoThread::run(){

    qDebug() << "Video Thread Running";
    bool toggle = true;

    for(;;){

        if(toggle){
            if(av_read_frame(formCtx,&packet) == 0){
                if(packet.stream_index == videostream1->index){

                    while(packet.size > 0){

                    int len = 0,framefinished = 0;

                     len = avcodec_decode_video2(vCodecCtx1, vFrame, &framefinished, &packet);

                    if(framefinished){

                    sws_scale(imgConvertCtx,vFrame->data,vFrame->linesize,
                                  0,vCodecCtx1->height,bgrFrame->data,bgrFrame->linesize);


                    emit sendData1(bgrFrame->data[0]);

                   // msleep(25);
                    }
                    packet.size -= len;
                    packet.data += len;
                    }
                }
                av_free_packet(&packet);
        }


    }else{
            if(av_read_frame(formatCtx2,&packet) == 0){
                if(packet.stream_index == videostream2->index){

                    while(packet.size > 0){

                    int len = 0,framefinished = 0;

                     len = avcodec_decode_video2(vCodecCtx2, vFrame2, &framefinished, &packet);

                    if(framefinished){


                    sws_scale(imgConvertCtx2,vFrame2->data,vFrame2->linesize,
                                  0,vCodecCtx2->height,bgrFrame2->data,bgrFrame2->linesize);


                    emit sendData2(bgrFrame2->data[0]);

                    //msleep(25);
                    }
                    packet.size -= len;
                    packet.data += len;
                    }
                }
                av_free_packet(&packet);

        }

    }
         toggle^= true;

  }

//    while(av_read_frame(formCtx,&packet) == 0){

//        if(packet.stream_index == videostream->index){

//            while(packet.size > 0){

//            int len = 0,framefinished = 0;

//             len = avcodec_decode_video2(vCodecCtx, vFrame, &framefinished, &packet);

//            if(framefinished){
//                //sws_scale(imgConvertCtx,(byte const* const*)vFrame->data,vFrame->linesize,0,vCodecCtx->height,vFrameRGB->data,vFrameRGB->linesize);

//            sws_scale(imgConvertCtx,vFrame->data,vFrame->linesize,
//                          0,vCodecCtx->height,bgrFrame->data,bgrFrame->linesize);


//            emit sendData(bgrFrame->data[0]);

//            msleep(25);

            // cv::Mat myFrame(vFrame->height, vFrame->width , CV_8UC3 ,bgrFrame->data[0]);

//               //Fill mat object with pixel data
//               for(int i = 0; i < vCodecCtx->height; i++){
//                   for(int j = 0; j < vCodecCtx->width; j++){
//                       myFrame.at<Vec3b>(i,j)[0] = bgrFrame->data[0][i * bgrFrame->linesize[0] + j * 3 + 0];
//                       myFrame.at<Vec3b>(i,j)[1] = bgrFrame->data[0][i * bgrFrame->linesize[0] + j * 3 + 1];
//                       myFrame.at<Vec3b>(i,j)[2] = bgrFrame->data[0][i * bgrFrame->linesize[0] + j * 3 + 2];
//                   }
//               }



               //if Frame is legitimate, append to vector of mats

//               if(!myFrame.empty()){


//                       cv::resize(myFrame,myAnalyticFrame,nSz);
//                       emit sendMat(myAnalyticFrame);
//                       //analytics.append(myAnalyticFrame);






//          }
              // qDebug() << analytics.size();
              // if(analytics.size() % 5 == 0 && AToggle == true){

             //          AToggle = false;


                      /* QSize sz;
                       sz.setHeight(destHeight);
                       sz.setWidth(destWidth);

                      // HCRateThread = new CutRateDetectionThread(analytics,sz);
                      // HCRateThread->setFPS(FPS);
                       HCRateThread->start();*/



                //   }else{
                //        AToggle = true;
                      // HCRateThread->addFrames();
                      // qDebug() << "Add more frames that were buffered here";

              // }
//        }
//            packet.size -= len;
//            packet.data += len;

//          }
//        }
//        av_free_packet(&packet);


//    }





   // QObject::connect(HCRateThread,SIGNAL(sendPixMap(QPixmap)),this,SLOT(sendPixHS(QPixmap)));


   /* for(int i = 0; i < pVec.size(); i++){
        emit sendPix(pVec.at(i));

        msleep(32);

    }*/



}

void VideoThread::initVideoFrame(){
    vFrame = avcodec_alloc_frame();
    vFrame->nb_samples = vCodecCtx1->frame_size;
    vFrame->format = vCodecCtx1->sample_fmt;
    vFrame->channel_layout = vCodecCtx1->channel_layout;

    vFrameRGB = avcodec_alloc_frame();
    vFrameRGB->nb_samples = vCodecCtx1->frame_size;
    vFrameRGB->format = AV_PIX_FMT_RGB24;
    vFrameRGB->channel_layout = vCodecCtx1->channel_layout;

    bgrFrame = avcodec_alloc_frame();
    bgrFrame->nb_samples = vCodecCtx1->frame_size;
    bgrFrame->format = AV_PIX_FMT_BGR24;
    bgrFrame->channel_layout = vCodecCtx1->channel_layout;

    bgrFrame2 = avcodec_alloc_frame();
    bgrFrame2->nb_samples = vCodecCtx2->channel_layout;
    bgrFrame2->format = AV_PIX_FMT_BGR24;
    bgrFrame2->channel_layout = vCodecCtx2->channel_layout;

    vFrame2 = avcodec_alloc_frame();
    vFrame2->nb_samples = vCodecCtx2->frame_size;
    vFrame2->format = vCodecCtx2->sample_fmt;
    vFrame2->channel_layout = vCodecCtx2->channel_layout;


}

void VideoThread::findVideoStream(){

    vidStream = av_find_best_stream(formCtx,AVMEDIA_TYPE_VIDEO,-1,-1,&vCodec,0);
    vidStream2 = av_find_best_stream(formatCtx2,AVMEDIA_TYPE_VIDEO,-1,-1,&vCodec2,0);

    if(vidStream < 0){
        qDebug() << "Could Not find Video Stream in Vid Thread";
    }else{
        videostream1 = formCtx->streams[vidStream];
        vCodecCtx1 = videostream1->codec;
        vCodecCtx1->codec = vCodec;

        //FPS = videostream->avg_frame_rate.num;
    }

    if(vidStream2 < 0){
        qDebug() << "Could not find video stream 2";
    }else{
        videostream2 = formatCtx2->streams[vidStream2];
        vCodecCtx2 = videostream2->codec;
        vCodecCtx2->codec = vCodec2;
    }

}

bool VideoThread::checkCodec()
{
    int checks = avcodec_open2(vCodecCtx1,vCodec,&cDict);
    int checks2 = avcodec_open2(vCodecCtx2,vCodec2,&cDict);

    if(checks < 0 && checks2 < 0)
        return false;
    else
        return true;
}

void VideoThread::initConverter()
{//sws_getContext(destWidth,destHeight,vCodecCtx->pix_fmt,destWidth,destHeight,AV_PIX_FMT_RGB24,SWS_BILINEAR,NULL,NULL,NULL);

    imgConvertCtx = sws_getContext(destWidth,destHeight,
                                   vCodecCtx1->pix_fmt,destWidth,destHeight,AV_PIX_FMT_BGR24,SWS_BICUBIC,NULL,NULL,NULL);
    imgConvertCtx2 = sws_getContext(destWidth2,destHeight2,vCodecCtx2->pix_fmt,
                                    destWidth2,destHeight2,AV_PIX_FMT_BGR24,SWS_BICUBIC,NULL,NULL,NULL);

    if(imgConvertCtx < 0)
        qDebug() << "Could not obtain sws convert context: VideoThread";

    if(imgConvertCtx2 < 0)
        qDebug() << "Could not obtain sws convert context2: Video Thread";
}

void VideoThread::setDstInts()
{
    destFmt = PIX_FMT_RGB;
    destHeight = vCodecCtx1->height;
    destWidth = vCodecCtx1->width;

    destHeight2 = vCodecCtx2->height;
    destWidth2 = vCodecCtx2->width;

    nSz.height = destHeight/16;
    nSz.width = destWidth/16;
}

void VideoThread::allocRGBPic()
{
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24,destWidth,destHeight);
    byte *buffer = (byte*)av_malloc(numBytes*sizeof(byte));
    int chek = avpicture_fill((AVPicture*)vFrameRGB,buffer,AV_PIX_FMT_RGB24,vCodecCtx1->width,vCodecCtx1->height);

    int numBgrBytes = avpicture_get_size(AV_PIX_FMT_BGR24,vCodecCtx1->width, vCodecCtx1->height);
    byte *bufBGR = (byte*)av_malloc(numBgrBytes*sizeof(byte));
    int chek2 = avpicture_fill((AVPicture*)bgrFrame,bufBGR,AV_PIX_FMT_BGR24,vCodecCtx1->width,vCodecCtx1->height);

    int numBgrBytes2 = avpicture_get_size(AV_PIX_FMT_BGR24,vCodecCtx2->width, vCodecCtx2->height);
    byte *bufBGR2 = (byte*)av_malloc(numBgrBytes2*sizeof(byte));
    int chek3 = avpicture_fill((AVPicture*)bgrFrame2,bufBGR2,AV_PIX_FMT_BGR24,vCodecCtx2->width,vCodecCtx2->height);

    if(chek < 0){
        qDebug() << "Could not fill vFrameRGB picture";
    }

    if(chek2 < 0){
        qDebug()<< "Could not fill bgrFrame picture";
    }

    if(chek3 < 0){
        qDebug() << "Could not fill bgrFrame2 picture";
    }


    //myFrame.create(destHeight,destWidth,CV_8UC3);

}

void VideoThread::sendPixVec()
{
    // emit sendPix(pVec);
}

void VideoThread::sendPixHS(QPixmap pix)
{
    emit sendPix(pix);
}

void VideoThread::sendMatsPB(Mat mat)
{
    emit sendMat(mat);
}






