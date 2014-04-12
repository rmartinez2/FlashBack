#include "videothread.h"

VideoThread::VideoThread(AVFormatContext *formCtx, playBackThread *thread, QObject *parent) :
    QThread(parent)
{
    this->formCtx = formCtx;
    //this->mutex = mutex;

    av_init_packet(&packet);



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


    playBack = thread;

   // connect(playBack,SIGNAL(sendMat(Mat)),this,SLOT(sendMatsPB(Mat)));

}

void VideoThread::run(){

    qDebug() << "Video Thread Running";
    //vBuf.open(QBuffer::WriteOnly);

    while(av_read_frame(formCtx,&packet) == 0)
    {

        if(packet.stream_index == videostream->index)
        {

            while(packet.size > 0)
            {

                int len = 0,framefinished = 0;

                len = avcodec_decode_video2(vCodecCtx, vFrame, &framefinished, &packet);

                if(framefinished)
                {
                //sws_scale(imgConvertCtx,(byte const* const*)vFrame->data,vFrame->linesize,0,vCodecCtx->height,vFrameRGB->data,vFrameRGB->linesize);

                    sws_scale(imgConvertCtx,vFrame->data,vFrame->linesize,
                            0,vCodecCtx->height,bgrFrame->data,bgrFrame->linesize);

                    cv::Mat myFrame(vFrame->height, vFrame->width , CV_8UC3 ,bgrFrame->data[0]);


                    if(!myFrame.empty()){

                            //qDebug() << "Not Empty! :D";
                           resize(myFrame,myAnalyticFrame,nSz);
                           emit sendMat(myAnalyticFrame);
                        }

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

    bgrFrame = avcodec_alloc_frame();
    bgrFrame->nb_samples = vCodecCtx->frame_size;
    bgrFrame->format = AV_PIX_FMT_BGR24;
    bgrFrame->channel_layout = vCodecCtx->channel_layout;


}

void VideoThread::findVideoStream()
{
    vidStream = av_find_best_stream(formCtx,AVMEDIA_TYPE_VIDEO,-1,-1,&vCodec,0);

    if(vidStream < 0)
    {
        qDebug() << "Could Not find Video Stream in Vid Thread";
    }
    else
    {
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

    nSz.height = destHeight/4;
    nSz.width = destWidth/4;
}

void VideoThread::allocRGBPic()
{
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24,destWidth,destHeight);
    uint8_t *buffer = (uint8_t*)av_malloc(numBytes*sizeof(uint8_t));
    int chek = avpicture_fill((AVPicture*)vFrameRGB,buffer,AV_PIX_FMT_RGB24,vCodecCtx->width,vCodecCtx->height);

    int numBgrBytes = avpicture_get_size(AV_PIX_FMT_BGR24,vCodecCtx->width, vCodecCtx->height);
    uint8_t *bufBGR = (uint8_t*)av_malloc(numBgrBytes*sizeof(uint8_t));
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

void VideoThread::sendMatsPB(Mat mat)
{
    emit sendMat(mat);
}


