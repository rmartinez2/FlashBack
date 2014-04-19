#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QString>
#include <QSize>

#include <time.h>


class NotificationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationWidget(QWidget *parent = 0);

    QSize sizeHint() const;
    void paintEvent(QPaintEvent *event);

    bool toggle;

    const char* TopText;
    const char* BottomText;

    time_t timer;


    int w,h,x,y;
    
signals:
    
public slots:
    void setXYWH(int x, int y, int w, int h);
    void setTopText(const char* topText);
    void setBottomText(const char* bottomText);
    void updateTime();
    
};

#endif // NOTIFICATIONWIDGET_H
