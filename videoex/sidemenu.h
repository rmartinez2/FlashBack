#ifndef SIDEMENU_H
#define SIDEMENU_H

#include <QWidget>
#include <QPainter>

class SideMenu : public QWidget
{
    Q_OBJECT
public:
    explicit SideMenu(QWidget *parent = 0);

    QSize sizeHint();
    void paintEvent(QPaintEvent *event);

    int x,y,w,h;
    
signals:
    
public slots:
    void setXYWH(int x, int y, int w, int h);
    
};

#endif // SIDEMENU_H
