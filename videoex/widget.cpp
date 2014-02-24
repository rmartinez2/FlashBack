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

    //Register all of the codecs from FFMPEG
    av_register_all();

    AVDictionary *optionsDict = NULL;

    //Open a video file and fill formCtx with its AVFormatContext
    if(avformat_open_input(&formCtx,"C:\\Users\\Rene\\Desktop\\apollo_17_stroll.mpg",NULL,NULL) != 0){
        qDebug() << "Did not open video";
    }

    //Find the stream info for the opened file
    if(avformat_find_stream_info(formCtx,NULL) < 0){
        qDebug() << "Couldn't find stream info";
    }


    //debugging info on video and stream
    av_dump_format(formCtx,0,"C:\\Users\\Rene\\Desktop\\apollo_17_stroll.mpg",0);


   streamIndex = -1;
   svIndex = -1;

   //Find the best Audio and Video stream
   streamIndex = av_find_best_stream(formCtx,AVMEDIA_TYPE_AUDIO,-1,-1,&pCodec,0);
   svIndex = av_find_best_stream(formCtx,AVMEDIA_TYPE_VIDEO,-1,-1,&vCodec,0);

    if(streamIndex > 0){
          //Set the Audio stream to the Audio stream found in the format context
          audioStream = formCtx->streams[streamIndex];

    }

    if(svIndex > 0){

        //Ser the Video stream to the Video stream found in the format context
        videoStream = formCtx->streams[svIndex];

        //Set the Codec Context codec to the codec found in the video steam
        vCodecCtx = videoStream->codec;
        vCodecCtx->codec = vCodec;


        //Create QRect object, set height and width of object to match dimensions of video
        QRect rect;
        rect.setHeight(vCodecCtx->height);
        rect.setWidth(vCodecCtx->width);

        //Set label to match the dimensions of the video
        ui->imageLab->setGeometry(rect);
    }

    //Create new avThread object
    avThread = new decodeAVThread(formCtx);

    //Connect finished pixel map to UI thread
    connect(avThread,SIGNAL(sendPix(QPixmap)),this,SLOT(setImgLab(QPixmap)));

    //Start thread
    avThread->start();






}

Widget::~Widget()
{
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


            ui->imageLab->setGeometry(rect);
            ui->imageLab->setPixmap(imMap);
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

void Widget::packetQueueInit(PacketQueue *q)
{
    q = new PacketQueue();
    q->unlockMutex();
}

int Widget::packetQueuePut(PacketQueue *q, AVPacket *packet)
{
    AVPacketList *pkt1;

    if(av_dup_packet(packet) < 0){
        qDebug() << "Packet error: packetQueuePut";
        return -1;
    }

    pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));

    if(!pkt1){
        qDebug() << "Error allocating packet list: packetQueuePut";
        return -1;
    }

    pkt1->pkt = *packet;
    pkt1->next = NULL;

    q->mutex.lock();

    if(!q->lastPkt)
        q->setFirstPkt(pkt1);

    else
        q->lastPkt->next = pkt1;

    q->setLastPkt(pkt1);
    q->nb_packets++;
    q->size += pkt1->pkt.size;

    q->cond->conditionSignal();
    q->mutex.unlock();

    return 0;

}

int Widget::packetQueueGet(PacketQueue *q, AVPacket *packet, int block)
{
    AVPacketList *pkt1;
    int ret;

    q->mutex.lock();

    for(;;){

    if(globalVideoState->quit){
        return -1;
        break;
    }

    pkt1 = q->firstPkt;

    if(pkt1){
        q->firstPkt = pkt1->next;

        if(!q->firstPkt)
            q->lastPkt = NULL;

        q->nb_packets--;
        q->size -= pkt1->pkt.size;

        *packet = pkt1->pkt;
        av_free(pkt1);
        ret = 1;
        break;
    }else if(!block){
        ret = 0;
        break;
    }else{
        qDebug() << "conditinal wait with semaphores";
        //conditionwait with semaphores
    }

    }
    q->mutex.unlock();
    return ret;

}

double Widget::getAudioClock(VideoState *is)
{
    double pts;
    int hw_buf_size, bytes_per_sec, n;

    pts = is->audio_clock;

    return pts;
}

void Widget::setImgLab(QPixmap pix)
{
    //qDebug() << "Got pixmap";
    ui->imageLab->setPixmap(pix);



}




void Widget::on_horizontalSlider_sliderMoved(int position)
{
    ui->horizontalSlider->setValue(position);
}
