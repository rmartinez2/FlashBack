#include "audiothread.h"

AudioThread::AudioThread(AVFormatContext *formCtx, QObject *parent) :
    QThread(parent)
{
    this->formCtx = formCtx;
   // this->mutex = mutex;
    av_init_packet(&packet);

    findAudioStream();
    initAudioFrame();
    getBufferSize();
    fillAudioFrame();
    setAudioFormat();
    setAudioDeviceInfo();

}

void AudioThread::run()
{
   buf.open(QBuffer::ReadWrite);
   qDebug() << "Audio Thread Started";


   while(av_read_frame(formCtx,&packet) == 0){
          qint64 written = 0;

            if(packet.stream_index == audioStream->index){ //audioStream->index){

                while(packet.size > 0){

                int len = 0,framefinished = 0, data_size;

                 len = avcodec_decode_audio4(aCodecCtx,aFrame,&framefinished,&packet);
                // qDebug() << framefinished;


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
            //sleep(int(1/aCodecCtx->sample_rate));
            // mutex.unlock();
            av_free_packet(&packet);
        }


  // qDebug() << "the end";
    if(detectSilence()){
     buf.seek(0);
     output->start(&buf);
    }


     //qDebug() << output->error();


}

bool AudioThread::findAudioStream()
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

void AudioThread::setAudioFormat()
{

    aFormat.setSampleRate(aCodecCtx->sample_rate);
    aFormat.setCodec("audio/pcm");
    aFormat.setChannelCount(aCodecCtx->channels);
    aFormat.setSampleSize(16);
    aFormat.setByteOrder(QAudioFormat::LittleEndian);
    aFormat.setSampleType(QAudioFormat::SignedInt);
}

bool AudioThread::setAudioDeviceInfo()
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

void AudioThread::finishedPlaying(QAudio::State state)
{
    if (state == QAudio::IdleState) {
        output->stop();
        //delete output;
    }
}

void AudioThread::initAudioFrame()
{
    aFrame = avcodec_alloc_frame();
    aFrame->nb_samples = aCodecCtx->frame_size;
    aFrame->format = aCodecCtx->sample_fmt;
    aFrame->channel_layout = aCodecCtx->channel_layout;
}

void AudioThread::getSampleSize()
{
}


void AudioThread::getBufferSize()
{
    bufferSize = av_samples_get_buffer_size(NULL,aCodecCtx->channels,aCodecCtx->frame_size,aCodecCtx->sample_fmt,0);

    sampleSize = (uint*)av_malloc(bufferSize);

    if(!sampleSize)
        qDebug() << "Sample size error";

}

void AudioThread::fillAudioFrame()
{
    int check = avcodec_fill_audio_frame(aFrame,aCodecCtx->channels,aCodecCtx->sample_fmt,(const UINT8*)sampleSize,bufferSize,0);
    if(check < 0){
        qDebug() << "Could not fill audio frame";
    }
}

bool AudioThread::detectSilence()
{
    int sum,mean,stdDev;
    QVector<int> sqHolder;

    for(int i = 0; i < buffData.size(); i++){
        sum += buffData.at(i);
    }

    mean = (int) (sum/buffData.size());

    for(int i = 0; i < buffData.size(); i++){
        stdDev += (buffData.at(i) - mean)^2 ;
    }

    stdDev /= (int) stdDev/buffData.size();

    //qDebug() << stdDev;

    if(stdDev >= 1150){
        qDebug() << "Silence";
        return false;
    }else{
        qDebug() << "Sound";
    return true;
    }


}
