#include "decodeavthread.h"

decodeAVThread::decodeAVThread(AVFormatContext *formCtx, QObject *parent) :
    QThread(parent)
{
    //Set this frames format context by pass by reference
    this->formCtx = formCtx;

    //Initialize the global packet container
    av_init_packet(&packet);

    //Find audio and video stream
    findVideoStream();
    findAudioStream();

    //Audio intializations
    initAudioFrame();
    getBufferSize();
    fillAudioFrame();
    setAudioFormat();
    setAudioDeviceInfo();

    //Video intializations
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

void decodeAVThread::run()
{
/*
 *
 *decodeAVThread run decodes both audio and video streams from a file
 *and sends signals for audio and video playback onto the widget
 *
 */

    buf.open(QBuffer::ReadWrite);

    while(av_read_frame(formCtx,&packet) == 0){

        if(packet.stream_index == videostream->index){

            while(packet.size > 0){

            int len = 0,framefinished = 0;

             len = avcodec_decode_video2(vCodecCtx, vFrame, &framefinished, &packet);

            if(framefinished){

               sws_scale(imgConvertCtx,(UINT8 const* const*)vFrame->data,vFrame->linesize,
                          0,vCodecCtx->height,vFrameRGB->data,vFrameRGB->linesize);


               QImage img(vFrameRGB->data[0],vCodecCtx->width,vCodecCtx->height,QImage::Format_RGB888);
               myImgs.append(img);
               QPixmap *pix = new QPixmap(destWidth,destHeight);
               pix->fromImage(img);
               pVec.push_back(*pix);


            }
            packet.size -= len;
            packet.data += len;
          }

        }else if(packet.stream_index == audioStream->index){

             qint64 written = 0;

            while(packet.size > 0){

            int len = 0,framefinished = 0, data_size;

             len = avcodec_decode_audio4(aCodecCtx,aFrame,&framefinished,&packet);

             if(framefinished){

                data_size = av_samples_get_buffer_size(NULL,aCodecCtx->channels,aFrame->nb_samples,aCodecCtx->sample_fmt,1);

                if(data_size > 0)

                    buffData.append((int) aFrame->data[0]);
                    written = buf.write((const char*)aFrame->data[0],data_size);
            }

            packet.size -= len;
            packet.data += len;


            }
        }
        av_free_packet(&packet);

    }

     //detect silence in the given frames of audio, play audio if silence is not detected
     if(detectSilence()){
     buf.seek(0);
     output->start(&buf);
     }

     //Run thread to detect black screen
     bsThread = new BSDetectionThread(myImgs);
     bsThread->start();

     //If audio has started begin play video
    if(output->state() == QAudio::ActiveState){

    for(int i = 0; i < pVec.size(); i++){
         emit sendPix(pVec.at(i));
         emit changeSlider(i);
         msleep(FPS+2);
    }
}



}

//Precondition: void
void decodeAVThread::initVideoFrame()
{
    vFrame = avcodec_alloc_frame();
    vFrame->nb_samples = vCodecCtx->frame_size;
    vFrame->format = vCodecCtx->sample_fmt;
    vFrame->channel_layout = vCodecCtx->channel_layout;

    vFrameRGB = avcodec_alloc_frame();
    vFrameRGB->nb_samples = vCodecCtx->frame_size;
    vFrameRGB->format = AV_PIX_FMT_RGB24;
    vFrameRGB->channel_layout = vCodecCtx->channel_layout;
}
//Postcondition: video frames are intialized

//Precondition: void
void decodeAVThread::findVideoStream()
{
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
//Postcondition: find the video stream than set up the video codec context

//Precondition: void
bool decodeAVThread::checkCodec()
{
    int checks = avcodec_open2(vCodecCtx,vCodec,&cDict);

    if(checks < 0)
        return false;
    else
        return true;

}
//PostCondition: returns a boolean. True if codec is open, false if not.

//Precondition: void
void decodeAVThread::initConverter()
{
    imgConvertCtx = sws_getContext(destWidth,destHeight,
                                   vCodecCtx->pix_fmt,destWidth,destHeight,AV_PIX_FMT_RGB24,SWS_BILINEAR,NULL,NULL,NULL);

    if(imgConvertCtx < 0)
        qDebug() << "Could not obtain sws convert context: VideoThread";
}
//Postcondition: Intialize SWScontext to convert raw data to RGB

//Precondition: void
void decodeAVThread::setDstInts()
{
    destFmt = PIX_FMT_RGB;
    destHeight = vCodecCtx->height;
    destWidth = vCodecCtx->width;
}
//Postcondition: set destination variables

//Precondition: void
void decodeAVThread::allocRGBPic()
{
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24,destWidth,destHeight);
    UINT8 *buffer = (UINT8*)av_malloc(numBytes*sizeof(UINT8));
    int chek = avpicture_fill((AVPicture*)vFrameRGB,buffer,AV_PIX_FMT_RGB24,vCodecCtx->width,vCodecCtx->height);

    if(chek < 0){
        qDebug() << "Could not fill vFrameRGB picture";
    }
}
//Postcondition: allocate rgb frame

void decodeAVThread::sendPixVec()
{
     // emit sendPix(pVec);
}

//Precondition: void
bool decodeAVThread::findAudioStream()
{

    aStream = av_find_best_stream(formCtx,AVMEDIA_TYPE_AUDIO,-1,-1,&aCodec,0);

    if(aStream < 0){
        qDebug() << "Error finding audio stream";
        return false;
    }else{
        audioStream = formCtx->streams[aStream];
        aCodecCtx = audioStream->codec;
        aCodecCtx->codec = aCodec;

       int checks = avcodec_open2(aCodecCtx,aCodec,&aDict);

       if(checks < 0)
           return false;

       return true;
    }
}
//Postcondition: find best audio stream, set up codec context and return true codec is legal

//Precondition: void
void decodeAVThread::setAudioFormat()
{
    aFormat.setSampleRate(aCodecCtx->sample_rate);
    aFormat.setCodec("audio/pcm");
    aFormat.setChannelCount(aCodecCtx->channels);
    aFormat.setSampleSize(16);
    aFormat.setByteOrder(QAudioFormat::LittleEndian);
    aFormat.setSampleType(QAudioFormat::SignedInt);
}
//Postcondition: intialize audioformat for playback

//Precondition: void
bool decodeAVThread::setAudioDeviceInfo()
{
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

    if(info.isFormatSupported(aFormat)){

    output = new QAudioOutput(aFormat);
    QObject::connect(output,SIGNAL(stateChanged(QAudio::State)),this,SLOT(finishedPlaying(QAudio::State)));
    output->setVolume(100);

    return true;

    }
     qDebug() << "format unsupported";
    return false;

}
//Postcondition: determine if audio format is supported, create new audio output object, set up connections
//return false if audio format is not supported

//Precondition: pass in current audio state
void decodeAVThread::finishedPlaying(QAudio::State state)
{
    if (state == QAudio::IdleState) {
        output->stop();
    }
}
//Postcondition: if audio is idle, stop audio output

//Precondition: void
void decodeAVThread::initAudioFrame()
{
    aFrame = avcodec_alloc_frame();
    aFrame->nb_samples = aCodecCtx->frame_size;
    aFrame->format = aCodecCtx->sample_fmt;
    aFrame->channel_layout = aCodecCtx->channel_layout;
}
//Postcondition: initialize audio frame

void decodeAVThread::getSampleSize()
{
}

//Precondition: void
void decodeAVThread::getBufferSize()
{
    bufferSize = av_samples_get_buffer_size(NULL,aCodecCtx->channels,aCodecCtx->frame_size,aCodecCtx->sample_fmt,0);

    sampleSize = (uint*)av_malloc(bufferSize);

    if(!sampleSize)
        qDebug() << "Sample size error";
}
//Postcondition: audio buffer gets initialized

//Precondition: void
void decodeAVThread::fillAudioFrame()
{
    int check = avcodec_fill_audio_frame(aFrame,aCodecCtx->channels,aCodecCtx->sample_fmt,(const UINT8*)sampleSize,bufferSize,0);
    if(check < 0){
        qDebug() << "Could not fill audio frame";
    }
}
//Postcondition: intialize audio frame and fill with data pointers

//Precondition: void
bool decodeAVThread::detectSilence()
{
    int sum,mean,stdDev;

    for(int i = 0; i < buffData.size(); i++){
        sum += buffData.at(i);
    }

    mean = (int) (sum/buffData.size());

    for(int i = 0; i < buffData.size(); i++){
        stdDev += (buffData.at(i) - mean)^2 ;
    }

    stdDev /= (int) stdDev/buffData.size();

    //For a 30 s video, the stdDev of silence is above 1150
    if(stdDev >= 1150){
        qDebug() << "Silence";
        return false;
    }else{
        qDebug() << "Sound";
    return true;
    }

}
//Postcondition: using statistics, determine if frames are silent, return false if silent


Mat decodeAVThread::QImagetoMat(QImage img)
{
    Mat temp(img.height(), img.width(), CV_8UC3, (uchar*) img.bits(), img.bytesPerLine());
    Mat result;
    cvtColor(temp,result,CV_BGR2RGB);

   // imshow("Test",result);
   // waitKey(0);

    return result;
}
