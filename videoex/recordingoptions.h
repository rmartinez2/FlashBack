#ifndef RECORDINGOPTIONS_H
#define RECORDINGOPTIONS_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>


class RecordingOptions : public QWidget
{
    Q_OBJECT
public:
    explicit RecordingOptions(QWidget *parent = 0);

    void paintEvent(QPaintEvent *event);
    QSize sizeHint();

    QPushButton *recordPrimary;
    QPushButton *recordSecondary;
    QPushButton *cancelAllRecording;

    bool primary,secondary,cancel;

    int x,y,w,h;



    
signals:
    void primaryRecord(bool);
    void secondaryRecord(bool);
    void cancelRecording();
    
public slots:
    void primaryClicked();
    void secondaryClicked();
    void cancelClicked();
    void setXYWH(int x, int y, int w, int h);
    
};

#endif // RECORDINGOPTIONS_H
