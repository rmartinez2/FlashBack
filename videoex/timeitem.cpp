#include "timeitem.h"

timeItem::timeItem(bool begin)
{
    timer = time(0);
    this->begin = begin;
}

QRectF timeItem::boundingRect() const
{
    return QRectF(x,y,w,h);
}

void timeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    tm* dt = localtime(&timer);

    if(begin){
        painter->drawText(boundingRect(),QString::number(dt->tm_hour) + ":00");
    }else{
        painter->drawText(boundingRect(),QString::number(dt->tm_hour+1) + ":00");
    }
}

void timeItem::setXYWH(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

void timeItem::setXYWH(QRectF rect)
{
    this->x = rect.x();
    this->y = rect.y();
    this->w = rect.width();
    this->h = rect.height();
}
