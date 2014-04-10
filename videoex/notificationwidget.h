#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H

#include <QWidget>
#include <QPainter>

class NotificationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationWidget(QWidget *parent = 0);

    QSize sizeHint() const;
    void paintEvent(QPaintEvent *event);

    bool toggle;

    int w,h,x,y;
    
signals:
    
public slots:
    void setXYWH(int x, int y, int w, int h);
    
};

#endif // NOTIFICATIONWIDGET_H
