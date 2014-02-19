#include "videothread.h"

VideoThread::VideoThread(AVFormatContext *formCtx,QObject *parent) :
    QThread(parent)
{
    this->formCtx = formCtx;
    //this->mutex = mutex;

    av_init_packet(&packet);

    findVideoStream();


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

        if(packet.stream_index == videostream->index){

            while(packet.size > 0){

            int len = 0,framefinished = 0;

             len = avcodec_decode_video2(vCodecCtx, vFrame, &framefinished, &packet);

            if(framefinished){

               sws_scale(imgConvertCtx,(UINT8 const* const*)vFrame->data,vFrame->linesize,
                          0,vCodecCtx->height,vFrameRGB->data,vFrameRGB->linesize);

               QImage img(vFrameRGB->data[0],vCodecCtx->width,vCodecCtx->height,QImage::Format_RGB888);


                QPixmap *pix = new QPixmap(destWidth,destHeight);
                pix->fromImage(img);
                pVec.push_back(*pix);

                if(!pix->isNull()){
                    emit sendPix(*pix);
                }


                msleep(FPS);

            }
            packet.size -= len;
            packet.data += len;
          }
        }
        av_free_packet(&packet);


    }



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
{
    imgConvertCtx = sws_getContext(destWidth,destHeight,
                                   vCodecCtx->pix_fmt,destWidth,destHeight,AV_PIX_FMT_RGB24,SWS_BILINEAR,NULL,NULL,NULL);

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
    UINT8 *buffer = (UINT8*)av_malloc(numBytes*sizeof(UINT8));
    int chek = avpicture_fill((AVPicture*)vFrameRGB,buffer,AV_PIX_FMT_RGB24,vCodecCtx->width,vCodecCtx->height);

    if(chek < 0){
        qDebug() << "Could not fill vFrameRGB picture";
    }
}

void VideoThread::sendPixVec()
{
   // emit sendPix(pVec);
}


