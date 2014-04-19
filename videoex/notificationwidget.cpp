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


    tm* dt = localtime(&timer);
    this->setWindowOpacity(0.75);
    painter.drawRect(this->geometry());

    if(TopText != NULL){
        QFont font("Helvetica",10);

        painter.setFont(font);

        int length = sizeof(TopText);

        QRect tRect(QPoint(35,20),QSize(length*40,20));

        painter.drawText(tRect,(QString)TopText);

    }

    if(BottomText != NULL){
        int length = sizeof(BottomText);

        QFont font("Times",8);

        painter.setFont(font);

        QRect tRect(QPoint(35,45),QSize(length*20,40));

        painter.drawText(tRect,(QString)BottomText);

    }

    QString noteTime = QString::number(dt->tm_hour) + ":" + QString::number(dt->tm_min);

    QFont font("Times", 15);

    painter.setFont(font);

    QRect tRect(QPoint(110,55), QSize(noteTime.length()*20,20));
    painter.drawText(tRect,noteTime);






}

void NotificationWidget::setXYWH(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    this->setGeometry(x,y,w,h);
}

void NotificationWidget::setTopText(const char *topText)
{
    this->TopText = topText;
}

void NotificationWidget::setBottomText(const char *bottomText)
{
    this->BottomText = bottomText;
}

void NotificationWidget::updateTime()
{
    timer = time(0);
}
