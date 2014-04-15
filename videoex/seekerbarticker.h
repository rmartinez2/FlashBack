#ifndef SEEKERBARTICKER_H
#define SEEKERBARTICKER_H

#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QTimer>
#include <QDebug>

#include <time.h>


class seekerBarTicker : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    seekerBarTicker(int minute);

    ~seekerBarTicker();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int x, y, w, h;

    bool painted;

    int minute;

    bool isPoint;

    time_t timer;
    QTimer *updateTimer;


    
signals:
    
public slots:
    void setXYWH(int x, int y, int w, int h);
    void setPaint(bool paintThis);
    void setIsPoint(bool isPoint);
    void timerUpdate();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    
};

#endif // SEEKERBARTICKER_H
