#include "recordingoptions.h"

RecordingOptions::RecordingOptions(QWidget *parent) :
    QWidget(parent)
{
    this->setVisible(false);
    recordPrimary = new QPushButton("&Record Primary",this);
    recordPrimary->move(90,30);

    recordSecondary = new QPushButton("&Record Secondary",this);
    recordSecondary->move(270,30);

    cancelAllRecording = new QPushButton("&Cancel All Recording",this);
    cancelAllRecording->move(450,30);

    primary = false;
    secondary = false;
    cancel = false;

    connect(recordPrimary,SIGNAL(clicked()),this,SLOT(primaryClicked()));
    connect(recordSecondary,SIGNAL(clicked()),this,SLOT(secondaryClicked()));
    connect(cancelAllRecording,SIGNAL(clicked()),this,SLOT(cancelClicked()));
}

void RecordingOptions::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    this->setWindowOpacity(0.75);
    painter.drawRect(this->geometry());

    painter.end();

}

QSize RecordingOptions::sizeHint()
    {
    return QSize(w,h);
}

void RecordingOptions::primaryClicked()
{
    primary ^= true;
    if(primary){
        recordPrimary->setText("&Cancel Primary");
        recordPrimary->update();
        emit primaryRecord(true);
    }else{
        recordPrimary->setText("&Record Primary");
        recordPrimary->update();
       emit primaryRecord(false);
    }
}

void RecordingOptions::secondaryClicked()
{
    secondary ^= true;
    if(secondary){
        recordSecondary->setText("&Cancel Secondary");
       recordSecondary->update();
       emit secondaryRecord(true);

    }else{
        recordSecondary->setText("&Record Secondary");
        recordSecondary->update();
        emit secondaryRecord(false);

    }
}

void RecordingOptions::cancelClicked()
{
    primary = false;
    secondary = false;

    recordPrimary->setText("&Record Primary");
    recordPrimary->update();

    recordSecondary->setText("&Record Secondary");
    recordSecondary->update();

    emit cancelRecording();


}


void RecordingOptions::setXYWH(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    this->setGeometry(x,y,w,h);
}
