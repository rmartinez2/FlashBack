#include "videothread.h"

VideoThread::VideoThread(AVFormatContext *formCtx,QObject *parent) :
    QThread(parent)
{
    this->formCtx = formCtx;
    //this->mutex = mutex;

    av_init_packet(&packet);



    findVideoStream();

    FT = false;

    if(checkCodec() == true){
    setDstInts();
    initVideoFrame();
    initConverter();
    allocRGBPic();
    }else{
        qDebug() << "Failed to open codec";
    }


    connect(this,SIGNAL(tFinished()),this,SLOT(sendPixVec()));

}

void VideoThread::run(){

    qDebug() << "Video Thread Running";
    //vBuf.open(QBuffer::WriteOnly);


    QMutexLocker locker(&mutex);
    while(av_read_frame(formCtx,&packet) == 0){

        glClear(GL_COLOR_BUFFER_BIT);


        if(packet.stream_index == videostream->index){

            while(packet.size > 0){

            int len = 0,framefinished = 0;

             len = avcodec_decode_video2(vCodecCtx, vFrame, &framefinished, &packet);

            if(framefinished){
                //sws_scale(imgConvertCtx,(byte const* const*)vFrame->data,vFrame->linesize,0,vCodecCtx->height,vFrameRGB->data,vFrameRGB->linesize);

            int ni = sws_scale(imgConvertCtx,vFrame->data,vFrame->linesize,
                          0,vCodecCtx->height,bgrFrame->data,bgrFrame->linesize);


            if(ni > 0){

               //Fill mat object with pixel data
               for(int i = 0; i < vCodecCtx->height; i++){
                   for(int j = 0; j < vCodecCtx->width; j++){
                       myFrame.at<Vec3b>(i,j)[0] = bgrFrame->data[0][i * bgrFrame->linesize[0] + j * 3 + 0];
                       myFrame.at<Vec3b>(i,j)[1] = bgrFrame->data[0][i * bgrFrame->linesize[0] + j * 3 + 1];
                       myFrame.at<Vec3b>(i,j)[2] = bgrFrame->data[0][i * bgrFrame->linesize[0] + j * 3 + 2];
                   }
               }



               //if Frame is legitimate, append to to vector of mats
               if(!myFrame.empty()){
                   myMats.append(myFrame);
            }
          }
            }
            packet.size -= len;
            packet.data += len;

          }
        }
        av_free_packet(&packet);


    }

    /*HCRateThread = new CutRateDetectionThread(pVec);
    QObject::connect(HCRateThread,SIGNAL(sendPixMap(QPixmap)),this,SLOT(sendPixHS(QPixmap)));
    HCRateThread->setFPS(FPS);
    HCRateThread->start();*/

   /* for(int i = 0; i < pVec.size(); i++){
        emit sendPix(pVec.at(i));

        msleep(32);

    }*/



}

void VideoThread::initVideoFrame(){
    vFrame = avcodec_alloc_frame();
    vFrame->nb_samples = vCodecCtx->frame_size;
    vFrame->format = vCodecCtx->sample_fmt;
    vFrame->channel_layout = vCodecCtx->channel_layout;

    vFrameRGB = avcodec_alloc_frame();
    vFrameRGB->nb_samples = vCodecCtx->frame_size;
    vFrameRGB->format = AV_PIX_FMT_RGB24;
    vFrameRGB->channel_layout = vCodecCtx->channel_layout;

    bgrFrame = avcodec_alloc_frame();
    bgrFrame->nb_samples = vCodecCtx->frame_size;
    bgrFrame->format = AV_PIX_FMT_BGR24;
    bgrFrame->channel_layout = vCodecCtx->channel_layout;


}

void VideoThread::findVideoStream(){

    vidStream = av_find_best_stream(formCtx,AVMEDIA_TYPE_VIDEO,-1,-1,&vCodec,0);

    if(vidStream < 0){
        qDebug() << "Could Not find Video Stream in Vid Thread";
    }else{
        videostream = formCtx->streams[vidStream];
        vCodecCtx = videostream->codec;
        vCodecCtx->codec = vCodec;
        FPS = videostream->avg_frame_rate.num;
    }

}

bool VideoThread::checkCodec()
{
    int checks = avcodec_open2(vCodecCtx,vCodec,&cDict);

    if(checks < 0)
        return false;
    else
        return true;
}

void VideoThread::initConverter()
{//sws_getContext(destWidth,destHeight,vCodecCtx->pix_fmt,destWidth,destHeight,AV_PIX_FMT_RGB24,SWS_BILINEAR,NULL,NULL,NULL);

    imgConvertCtx = sws_getContext(destWidth,destHeight,
                                   vCodecCtx->pix_fmt,destWidth,destHeight,AV_PIX_FMT_BGR24,SWS_BICUBIC,NULL,NULL,NULL);

    if(imgConvertCtx < 0)
        qDebug() << "Could not obtain sws convert context: VideoThread";
}

void VideoThread::setDstInts()
{
    destFmt = PIX_FMT_RGB;
    destHeight = vCodecCtx->height;
    destWidth = vCodecCtx->width;
}

void VideoThread::allocRGBPic()
{
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24,destWidth,destHeight);
    byte *buffer = (byte*)av_malloc(numBytes*sizeof(byte));
    int chek = avpicture_fill((AVPicture*)vFrameRGB,buffer,AV_PIX_FMT_RGB24,vCodecCtx->width,vCodecCtx->height);

    int numBgrBytes = avpicture_get_size(AV_PIX_FMT_BGR24,vCodecCtx->width, vCodecCtx->height);
    byte *bufBGR = (byte*)av_malloc(numBgrBytes*sizeof(byte));
    int chek2 = avpicture_fill((AVPicture*)bgrFrame,bufBGR,AV_PIX_FMT_BGR24,vCodecCtx->width,vCodecCtx->height);

    if(chek < 0){
        qDebug() << "Could not fill vFrameRGB picture";
    }

    if(chek2 < 0){
        qDebug()<< "Could not fill bgrFrame picture";
    }

    myFrame.create(destHeight,destWidth,CV_8UC3);

}

void VideoThread::sendPixVec()
{
    // emit sendPix(pVec);
}

void VideoThread::sendPixHS(QPixmap pix)
{
    emit sendPix(pix);
}


