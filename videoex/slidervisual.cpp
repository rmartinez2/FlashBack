#include "slidervisual.h"

SliderVisual::SliderVisual(QWidget *parent) :
    QWidget(parent)
{
    this->setVisible(false);

    playToggle = true;

    QRect rect;
    rect.setRect(150,10,450,50);

    seekerView = new QGraphicsView(this);
    seekerView->setGeometry(rect);


    ff = new QPushButton(QIcon("C:\\Users\\Rene\\Pictures\\FF.png"),"",this);
    rw = new QPushButton(QIcon("C:\\Users\\Rene\\Pictures\\RW.png"),"",this);
    play = new QPushButton(QIcon("C:\\Users\\Rene\\Pictures\\play.png"),"",this);
    stop = new QPushButton(QIcon("C:\\Users\\Rene\\Pictures\\Stop.png"),"",this);
    pause = new QPushButton(QIcon("C:\\Users\\Rene\\Pictures\\pause.png"),"",this);

    ff->move(110,20);
    rw->move(20,20);
    play->move(50,20);
    pause->move(50,20);
    stop->move(80,20);

    play->setVisible(false);

    connect(ff,SIGNAL(clicked()),this,SLOT(vidFF()));
    connect(rw,SIGNAL(clicked()),this,SLOT(vidRW()));
    connect(play,SIGNAL(clicked()),this,SLOT(vidPause()));
    connect(pause,SIGNAL(clicked()),this,SLOT(vidPlay()));
    connect(stop,SIGNAL(clicked()),this,SLOT(vidStop()));

    qDebug() << this->updatesEnabled();



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
    //emit signal to pause playback
    playToggle = true;
    update();


}

void SliderVisual::vidPlay()
{
    playToggle = false;
    //emit signal to continue playback
    update();

}

void SliderVisual::vidFF()
{
}


void SliderVisual::vidStop()
{
}

void SliderVisual::vidRW()
{
}
