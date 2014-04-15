#ifndef SLIDERVISUAL_H
#define SLIDERVISUAL_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QIcon>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "seekerbar.h"
#include "seekerbarticker.h"
#include "timeitem.h"


class SliderVisual : public QWidget
{
    Q_OBJECT
public:
    explicit SliderVisual(QWidget *parent = 0);

    QSize sizeHint() const;

    int x,y,w,h;

    bool playToggle;

    void paintEvent(QPaintEvent *event);

    QPushButton *ff;
    QPushButton *rw;
    QPushButton *stop;
    QPushButton *play;
    QPushButton *pause;

    QGraphicsView *seekerView;
    QGraphicsScene *seekerScene;

    SeekerBar *seekerBar;

    seekerBarTicker *min0,*min5,*min10,*min15,*min20,*min25,*min30,*min35,*min40,*min45,*min50,*min55,*min60;

    timeItem *start, *end;



    
signals:
    void sendPause(bool);
    void sendPlay(bool);
    
public slots:

    void setXYWH(int x, int y, int w, int h);
    void vidPause();
    void vidPlay();
    void vidFF();
    void vidStop();
    void vidRW();

    QRectF timeRect1();
    QRectF timeRect2();
    
};

#endif // SLIDERVISUAL_H
