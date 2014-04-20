#ifndef SIDEMENU_H
#define SIDEMENU_H

#include <QtGui>
#include <QPainter>
#include <QPushButton>

class SideMenu : public QWidget
{
    Q_OBJECT
public:
    explicit SideMenu(QWidget *parent = 0);

    QSize sizeHint();
    void paintEvent(QPaintEvent *event);

    int x,y,w,h;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;


//    QPushButton *button4;
//    QPushButton *button5;
//    QPushButton *button6;


    bool fbToggle;
    
signals:
    void toggleFB(bool);
    void showRecordings(bool);
    void showRecordingOpts(bool);
    
public slots:
    void setXYWH(int x, int y, int w, int h);
    void fbToggled();
    void showRecordingsMenu();
    void showRecordingSettings();

    
};

#endif // SIDEMENU_H
