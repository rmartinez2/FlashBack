#include "seekerbar.h"

SeekerBar::SeekerBar()
{
    timer = time(0);
    seekerTicks = new QList<seekerBarTicker*>();

    min0 = new seekerBarTicker(0);
    min0->setIsPoint(true);
    min0->setXYWH(52,12,20,20);

    min30 = new seekerBarTicker(30);
    min30->setIsPoint(true);
    min30->setXYWH(215,12,20,20);

    min60 = new seekerBarTicker(60);
    min60->setIsPoint(true);
    min60->setXYWH(370,12,20,20);

    min5 = new seekerBarTicker(5);
    min5->setXYWH(70,17,30,10);

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

    seekerTicks->append(min0);
    seekerTicks->append(min5);
    seekerTicks->append(min10);
    seekerTicks->append(min15);
    seekerTicks->append(min20);
    seekerTicks->append(min25);
    seekerTicks->append(min30);
    seekerTicks->append(min35);
    seekerTicks->append(min40);
    seekerTicks->append(min45);
    seekerTicks->append(min50);
    seekerTicks->append(min55);
    seekerTicks->append(min60);
}

QRectF SeekerBar::boundingRect() const
{
    return QRectF(x,y,w,h);

}

void SeekerBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //52,35,20,10 for text 1
    //370,35,20,10 for text 2


    painter->setPen(Qt::black);

    painter->drawRect(345,17,30,10);
    painter->drawRect(315,17,30,10);
    painter->drawRect(285,17,30,10);
    painter->drawRect(255,17,30,10);
    painter->drawRect(225,17,30,10);


    painter->drawRect(190,17,30,10);
    painter->drawRect(160,17,30,10);
    painter->drawRect(130,17,30,10);
    painter->drawRect(100,17,30,10);
    painter->drawRect(70,17,30,10);



    painter->drawEllipse(370,12,20,20);
    painter->drawEllipse(215,12,20,20);
    painter->drawEllipse(52,12,20,20);




    tm* dt = localtime(&timer);
    QString time1 = QString::number(dt->tm_hour) + ":00";
    QString time2 = QString::number(dt->tm_hour + 1) + ":00";

    painter->drawText(getRect1(),time1);
    painter->drawText(getRect2(),time2);



}

void SeekerBar::setXYWH(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

QRectF SeekerBar::getRect1()
{
    return QRectF(50,35,70,30);
}

QRectF SeekerBar::getRect2()
{
    return QRectF(370,35,70,30);
}

void SeekerBar::fillSeeker()
{

}
