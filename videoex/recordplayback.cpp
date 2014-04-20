#include "recordplayback.h"

RecordPlayBack::RecordPlayBack(AVFormatContext *formCtx, QObject *parent) :
    QThread(parent)
{
    this->formCtx = formCtx;

    av_init_packet(&packet);

    findVideoStream();

    if(checkCodec() == true){
    setDstInts();
    initVideoFrame();
    initConverter();
    allocBGRPic();
    }else{
        qDebug() << "Failed to open codec";
    }
}

RecordPlayBack::~RecordPlayBack()
{
    avformat_free_context(formCtx);
    av_free_packet(&packet);

   // avcodec_free_frame()
}

void RecordPlayBack::run()
{
    qDebug() << "Record PlayBack Thread Running";

            while(av_read_frame(formCtx,&packet) == 0){
                if(packet.stream_index == videostream->index){

                    while(packet.size > 0){

                    int len = 0,framefinished = 0;

                     len = avcodec_decode_video2(vCodecCtx, vFrame, &framefinished, &packet);

                    if(framefinished){

                    sws_scale(imgConvertCtx,vFrame->data,vFrame->linesize,
                                  0,vCodecCtx->height,bgrFrame->data,bgrFrame->linesize);


                    emit sendFrameRecording(bgrFrame->data[0]);



                   // msleep(5);
                    }
                    packet.size -= len;
                    packet.data += len;
                    }
                }
                av_free_packet(&packet);
        }
}

void RecordPlayBack::initVideoFrame()
{
    vFrame = avcodec_alloc_frame();
    vFrame->nb_samples = vCodecCtx->frame_size;
    vFrame->format = vCodecCtx->sample_fmt;
    vFrame->channel_layout = vCodecCtx->channel_layout;

    bgrFrame = avcodec_alloc_frame();
    bgrFrame->nb_samples = vCodecCtx->frame_size;
    bgrFrame->format = AV_PIX_FMT_BGR24;
    bgrFrame->channel_layout = vCodecCtx->channel_layout;
}

void RecordPlayBack::findVideoStream()
{
    vidStream = av_find_best_stream(formCtx,AVMEDIA_TYPE_VIDEO,-1,-1,&vCodec,0);


    if(vidStream < 0){
        qDebug() << "Could Not find Video Stream in Vid Thread";
    }else{
        videostream = formCtx->streams[vidStream];
        vCodecCtx = videostream->codec;
        vCodecCtx->codec = vCodec;
    }

}

bool RecordPlayBack::checkCodec()
{
    int checks = avcodec_open2(vCodecCtx,vCodec,&cDict);

    if(checks < 0)
        return false;
    else
        return true;
}

void RecordPlayBack::initConverter()
{
    imgConvertCtx = sws_getContext(destWidth,destHeight,
                                   vCodecCtx->pix_fmt,1280,720,AV_PIX_FMT_BGR24,SWS_BICUBIC,NULL,NULL,NULL);

    if(imgConvertCtx < 0)
        qDebug() << "Could not obtain sws convert context: Record PlayBack Thread";


}

void RecordPlayBack::setDstInts()
{
    destHeight = vCodecCtx->height;
    destWidth = vCodecCtx->width;
}

void RecordPlayBack::allocBGRPic()
{
    int numBgrBytes = avpicture_get_size(AV_PIX_FMT_BGR24,vCodecCtx->width, vCodecCtx->height);
    uint8_t *bufBGR = (uint8_t*)av_malloc(numBgrBytes*sizeof(uint8_t));
    int chek = avpicture_fill((AVPicture*)bgrFrame,bufBGR,AV_PIX_FMT_BGR24,vCodecCtx->width,vCodecCtx->height);

    if(chek < 0){
        qDebug() << "Could not fill vFrameRGB picture";
    }


}
