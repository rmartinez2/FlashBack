#include "slidervisual.h"

/* need to set a timer to update slider ticker every minute */

SliderVisual::SliderVisual(QWidget *parent) :
    QWidget(parent)
{
    this->setVisible(false);

    playToggle = true;

    QRect rect;
    rect.setRect(150,10,450,50);

    seekerView = new QGraphicsView(this);
    seekerView->setAlignment(Qt::AlignCenter);

    seekerScene = new QGraphicsScene();
    seekerView->setGeometry(rect);
    seekerView->setScene(seekerScene);

    start = new timeItem(true);
    end = new timeItem(false);

    start->setXYWH(timeRect1());
    end->setXYWH(timeRect2());

    min5 = new seekerBarTicker(5);
    min5->setXYWH(70,17,30,10);

    min0 = new seekerBarTicker(0);
    min0->setIsPoint(true);
    min0->setXYWH(52,12,20,20);

    min30 = new seekerBarTicker(30);
    min30->setIsPoint(true);
    min30->setXYWH(215,12,20,20);

    min60 = new seekerBarTicker(60);
    min60->setIsPoint(true);
    min60->setXYWH(370,12,20,20);

    min10 = new seekerBarTicker(10);
    min10->setXYWH(100,17,30,10);

    min15 = new seekerBarTicker(15);
    min15->setXYWH(130,17,30,10);

    min20 = new seekerBarTicker(20);
    min20->setXYWH(160,17,30,10);

    min25 = new seekerBarTicker(25);
    min25->setXYWH(190,17,30,10);

    min35 = new seekerBarTicker(35);
    min35->setXYWH(225,17,30,10);

    min40 = new seekerBarTicker(40);
    min40->setXYWH(255,17,30,10);

    min45 = new seekerBarTicker(45);
    min45->setXYWH(285,17,30,10);

    min50 = new seekerBarTicker(50);
    min50->setXYWH(315,17,30,10);

    min55 = new seekerBarTicker(55);
    min55->setXYWH(345,17,30,10);


    seekerScene->addItem(min5);
    seekerScene->addItem(min10);
    seekerScene->addItem(min15);
    seekerScene->addItem(min20);
    seekerScene->addItem(min25);

    seekerScene->addItem(min35);
    seekerScene->addItem(min40);
    seekerScene->addItem(min45);
    seekerScene->addItem(min50);
    seekerScene->addItem(min55);

    seekerScene->addItem(min0);
    seekerScene->addItem(min30);
    seekerScene->addItem(min60);


//    ff = new QPushButton(QIcon("C:\\Users\\Rene\\Pictures\\FF.png"),"",this);
//    rw = new QPushButton(QIcon("C:\\Users\\Rene\\Pictures\\RW.png"),"",this);
    play = new QPushButton(QIcon("C:\\Users\\Rene\\Pictures\\play.png"),"",this);
//    stop = new QPushButton(QIcon("C:\\Users\\Rene\\Pictures\\Stop.png"),"",this);
    pause = new QPushButton(QIcon("C:\\Users\\Rene\\Pictures\\pause.png"),"",this);

//    ff->move(110,20);
//    rw->move(20,20);
    play->move(50,20);
    pause->move(50,20);
//    stop->move(80,20);

    play->setVisible(false);

//    connect(ff,SIGNAL(clicked()),this,SLOT(vidFF()));
//    connect(rw,SIGNAL(clicked()),this,SLOT(vidRW()));
    connect(play,SIGNAL(clicked()),this,SLOT(vidPause()));
    connect(pause,SIGNAL(clicked()),this,SLOT(vidPlay()));
//    connect(stop,SIGNAL(clicked()),this,SLOT(vidStop()));


    seekerScene->addItem(start);
    seekerScene->addItem(end);

   // seekerBar = new SeekerBar();
   // seekerBar->setXYWH(0,0,25,25);

  //  seekerScene->addItem(seekerBar);




}

QSize SliderVisual::sizeHint() const
{
    return QSize(w,h);
}

void SliderVisual::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    this->setWindowOpacity(0.75);
    painter.drawRect(this->geometry());

    if(playToggle == true){
        play->setVisible(false);
        pause->setVisible(true);

    }

    if(playToggle == false){
        play->setVisible(true);
        pause->setVisible(false);
    }

}

void SliderVisual::setXYWH(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    this->setGeometry(x,y,w,h);
}

void SliderVisual::vidPause()
{
    //emit signal to pause playback, stop ff or rw
    playToggle = true;
    emit sendPlay(false);
   // qDebug() << "pause";
    update();


}

void SliderVisual::vidPlay()
{
    playToggle = false;
    //emit signal to continue playback, stop ff or rw
    emit sendPause(true);
    //qDebug() << "play";
    update();

}

void SliderVisual::vidFF()
{
    //emit FF signal here
}


void SliderVisual::vidStop()
{
    //emit stop signal here, stop ff or rw, or play
}

void SliderVisual::vidRW()
{
    //emit rewind signal here
}

QRectF SliderVisual::timeRect1()
{
    return QRectF(45,35,70,30);
}

QRectF SliderVisual::timeRect2()
{
    return QRectF(365,35,70,30);
}
