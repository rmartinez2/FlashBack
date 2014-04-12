#include "sidemenu.h"

SideMenu::SideMenu(QWidget *parent) :
    QWidget(parent)
{
}

QSize SideMenu::sizeHint()
{
    return QSize(w,h);
}

void SideMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.drawRect(this->geometry());
}

void SideMenu::setXYWH(int x, int y, int w, int h)
{

    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    this->setGeometry(x,y,w,h);
}
