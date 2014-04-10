#ifndef SLIDERVISUAL_H
#define SLIDERVISUAL_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QIcon>
#include <QDebug>
#include <QGraphicsView>


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



    
signals:
    
public slots:

    void setXYWH(int x, int y, int w, int h);
    void vidPause();
    void vidPlay();
    void vidFF();
    void vidStop();
    void vidRW();
    
};

#endif // SLIDERVISUAL_H
