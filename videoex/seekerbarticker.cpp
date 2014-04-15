#include "seekerbarticker.h"

seekerBarTicker::seekerBarTicker(int minute)
{
    this->minute = minute;
    this->isPoint = false;
    this->painted = false;

    updateTimer = new QTimer(this);
    updateTimer->setInterval(6000);

    connect(updateTimer,SIGNAL(timeout()),this,SLOT(timerUpdate()));

    timer = time(0);

    updateTimer->start();

}

seekerBarTicker::~seekerBarTicker()
{
    delete updateTimer;
}

QRectF seekerBarTicker::boundingRect() const
{
    return QRectF(x,y,w,h);
}

void seekerBarTicker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(Qt::black);

    tm *dt = localtime(&timer);

   // qDebug() << dt->tm_min << " " << this->minute;

    if(dt->tm_min >= this->minute){

        painter->setBrush(Qt::yellow);
    }else{
        painter->setBrush(Qt::white);
    }

    if(isPoint){
        painter->drawEllipse(boundingRect());
    }else{
    painter->drawRect(boundingRect());
    }

}

void seekerBarTicker::setXYWH(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    update();
}

void seekerBarTicker::setPaint(bool paintThis)
{
    painted = paintThis;
    update();
}

void seekerBarTicker::setIsPoint(bool isPoint)
{
    this->isPoint = isPoint;
}

void seekerBarTicker::timerUpdate()
{
   // qDebug() << "Timer update " << this->minute;

}

void seekerBarTicker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}
