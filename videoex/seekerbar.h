#ifndef SEEKERBAR_H
#define SEEKERBAR_H

#include <QGraphicsItem>
#include <QtCore>
#include <QPainter>
#include <QObject>
#include <time.h>
#include "seekerbarticker.h"
#include <QList>

class SeekerBar : public QObject, public QGraphicsItem
{
 Q_OBJECT
public:
    SeekerBar();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int x,y,w,h;

    time_t timer;

    QList<seekerBarTicker*> *seekerTicks;

    seekerBarTicker *min0,*min5,*min10,*min15,*min20,*min25,*min30,*min35,*min40,*min45,*min50,*min55,*min60;


    
signals:
    
public slots:

    void setXYWH(int x, int y, int w, int h);
    QRectF getRect1();
    QRectF getRect2();
    void fillSeeker();

protected:

    
};

#endif // SEEKERBAR_H
