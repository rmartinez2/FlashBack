#include "audiothread.h"
#include <inttypes.h>
#include <sys/types.h>

//possible way to scale up image up...change image to cv mat...scale up to appropriate dimensions than return back to
//qimage.


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
        //  qDebug() << "Read Frame";

            if(packet.stream_index == audioStream->index){ //audioStream->index){

                while(packet.size > 0){
                   // qDebug() << "in while packet " << packet.size;

                int len = 0,framefinished = 0, data_size;

                 len = avcodec_decode_audio4(aCodecCtx,aFrame,&framefinished,&packet);
                // qDebug() << framefinished;

                 if(len < 0){
                     qDebug() << "Error while decoding";
                 }


                 if(framefinished){

                     data_size = av_samples_get_buffer_size(NULL,aCodecCtx->channels,aFrame->nb_samples,aCodecCtx->sample_fmt,0);

                    if(data_size < 0){
                        qDebug() << "Failed to calculate data size";
                    }

                    if(data_size > 0){
                       // qDebug() << aFrame->data[0];


                        if(aFrame->channels < 2){

                        buffData.append((int) aFrame->data[0]);
                        written = buf.write((const char*)aFrame->data[0],data_size);

                        }
                        else{
                            interLeave(aFrame);
                            //buf.write((const char*) interLeave(aFrame), data_size);

                          /* INT64 channelLayout = av_get_default_channel_layout(aFrame->channels);
                           int ret, ret2;

                           AVSampleFormat format = (AVSampleFormat) aFrame->format;

                          SwrContext *swr = swr_alloc_set_opts(NULL,channelLayout,
                                                                av_get_packed_sample_fmt(format),
                                                                aFrame->sample_rate,channelLayout,format,
                                                                aFrame->sample_rate,0,NULL);

                           ret2 = swr_init(swr);

                           if(ret2 < 0)
                               qDebug() << "Error initializing swrctx";

                           byte* output[] = {sampleSize};

                           ret2 = swr_convert(swr,output,aFrame->nb_samples,
                                              (const byte**) aFrame->extended_data, aFrame->nb_samples);

                           swr_free(&swr);

                           data_size = ret2 * aFrame->channels * av_get_bytes_per_sample(format);

                           written = buf.write((const char*) aFrame->data[0],data_size);

                           if(written < 0)
                               qDebug() << "error writing to buffer";*/


                        }



                }

                packet.size -= len;
                packet.data += len;

                }
            }
            //sleep(int(1/aCodecCtx->sample_rate));
            // mutex.unlock();
            av_free_packet(&packet);
        }
   }


  // qDebug() << "the end";
   // if(detectSilence()){
     //buf.seek(0);
    //output->start(&buf);
   // }


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

    sampleSize = (byte*)av_malloc(bufferSize);

    if(!sampleSize)
        qDebug() << "Sample size error";

}

void AudioThread::fillAudioFrame()
{
    int check = avcodec_fill_audio_frame(aFrame,aCodecCtx->channels,aCodecCtx->sample_fmt,
                                         (const byte*)sampleSize,bufferSize,0);
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

uint* AudioThread::interLeave(AVFrame *frame)
{
    QByteArray* Left =  (QByteArray*) frame->data[0];
    QByteArray* Right = (QByteArray*) frame->data[1];


    int lSize = sizeof(Left);
    int rSize = sizeof(Right);

   // qDebug() << lSize << " " << rSize;

   // qDebug() << Left << " " << Right;

    QByteArray* Temp;



    qDebug() << Temp << " " << Left;

   // int i = Temp->size();

   // qDebug() << Temp;



   // Temp->append(Left);
   // Temp->append(Right);

    //qDebug() << "Left bS: " << Left << " " << Temp;

   // Left <<= 2;

   // qDebug() << "Left: " << Left;

   // Temp << 2;

   // Temp |= Right;

    //qDebug() << "Right: " <<(uint*) Temp << " " << (uint*) Right;



    //Temp << 4;

    //Temp |= Right;


    //qDebug() << (uint*)Temp;

    //return (uint*) Temp;



}
