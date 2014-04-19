#include "sidemenu.h"

SideMenu::SideMenu(QWidget *parent) :
    QWidget(parent)
{
    this->setVisible(false);
    button1 = new QPushButton("&Disable FlashBack",this);
    button1->move(40,75);

    button2 = new QPushButton("&Recordings",this);
    button2->move(40,150);

    button3 = new QPushButton("&Record Options",this);
    button3->move(40,225);

    fbToggle = true;

//    button4 = new QPushButton("&Notifications",this);
//    button4->move(40,200);

//    button5 = new QPushButton("&Other Settings",this);
//    button5->move(40,250);

    connect(button1,SIGNAL(clicked()),this,SLOT(fbToggled()));
    connect(button2,SIGNAL(clicked()),this,SLOT(showRecordingsMenu()));
    connect(button3,SIGNAL(clicked()),this,SLOT(showRecordingSettings()));
 //   connect(button4,SIGNAL(clicked()),this,SLOT());
//    connect(button5,SIGNAL(clicked()),this,SLOT());


}

QSize SideMenu::sizeHint()
{
    return QSize(w,h);
}

void SideMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    this->setWindowOpacity(0.75);
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

void SideMenu::fbToggled()
{

    fbToggle ^= true;
    if(fbToggle){
        button1->setText("&Disable FlashBack");
        button1->update();
        emit toggleFB(fbToggle);
        //turn on flashback
    }else{
        button1->setText("&Enable FlashBack");
        button1->update();
        emit toggleFB(fbToggle);
        //turn off flashback
    }
}

void SideMenu::showRecordingsMenu()
{

    this->setVisible(false);
    emit showRecordings(true);
}


void SideMenu::showRecordingSettings()
{
    this->setVisible(false);
    emit showRecordingOpts(true);
}


