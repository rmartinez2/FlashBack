#ifndef RECORDINGS_H
#define RECORDINGS_H

#include <QWidget>
#include <QDir>
#include <QFileInfoList>
#include <QPainter>
#include <QListWidget>
#include <QListWidgetItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "cvmatviewer.h"
#include <QPushButton>


class Recordings : public QWidget
{
    Q_OBJECT
public:
    explicit Recordings(QWidget *parent = 0);

    QDir recordDir;
    QFileInfoList recordList;

    QString myRecordDir;

    QSize sizeHint();

    void paintEvent(QPaintEvent *event);

    QListWidget *listWidget;

    cvMatViewer *littlePicture;

    QGraphicsView *PIPView;
    QGraphicsScene *PIPScene;
    QPushButton *exitButton;


    int h,w,x,y;
    
signals:
    void outofPIP(bool);
    void sendForPB(const char*);

    
public slots:
    void setRecordDir(QString myRecordDir);
    void setPIP(cvMatViewer *currentStream);
    void setXYWH(int x, int y, int w, int h);
    void createList();
    void sendPlayBackPath(QListWidgetItem *item);

    void exitScreen();
    
};

#endif // RECORDINGS_H
