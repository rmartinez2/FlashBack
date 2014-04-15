#ifndef TIMEITEM_H
#define TIMEITEM_H

#include <QGraphicsTextItem>
#include <QPainter>
#include <time.h>

class timeItem : public QObject, public QGraphicsTextItem
{
public:
    timeItem(bool begin);

    time_t timer;

    int x,y,w,h;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool begin;

public slots:
    void setXYWH(int x, int y, int w, int h);
    void setXYWH(QRectF rect);

};

#endif // TIMEITEM_H
