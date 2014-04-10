#include "notificationwidget.h"

NotificationWidget::NotificationWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setVisible(false);
}

QSize NotificationWidget::sizeHint() const
{
    return QSize(w,h);
}

void NotificationWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    this->setWindowOpacity(0.75);
    painter.drawRect(this->geometry());
}

void NotificationWidget::setXYWH(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    this->setGeometry(x,y,w,h);
}
