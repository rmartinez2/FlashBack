#include "recordings.h"

Recordings::Recordings( QWidget *parent) :
    QWidget(parent)
{
    this->setVisible(false);

   QRect rect(10,10,1280/6,720/6);

   recordDir.setFilter(QDir::Files);
   recordDir.setSorting(QDir::Size | QDir::Reversed);
   listWidget = new QListWidget(this);
   listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

   PIPView = new QGraphicsView(this);
   PIPScene = new QGraphicsScene();

   PIPView->setGeometry(rect);
   PIPView->setScene(PIPScene);

   //exitButton = new QPushButton("&Exit",this);

  // connect(exitButton,SIGNAL(clicked()),this,SLOT(exitScreen()));

   //PIPView->setVisible(false);


   connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(sendPlayBackPath(QListWidgetItem*)));


}

QSize Recordings::sizeHint()
{
    return QSize(w,h);
}

void Recordings::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    listWidget->setGeometry(10,130,600,200);
    painter.drawRect(this->geometry());

    //PIPScene->addWidget(this->littlePicture);

    painter.end();

}

void Recordings::setRecordDir(QString myRecordDir)
{
    this->myRecordDir = myRecordDir;
    recordDir.setPath(myRecordDir);
    recordList = recordDir.entryInfoList();

    listWidget->clear();
    createList();
}

void Recordings::setPIP(cvMatViewer *currentStream)
{
    this->littlePicture = currentStream;
    PIPScene->addWidget(littlePicture);


}

void Recordings::setXYWH(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    this->setGeometry(x,y,w,h);
}

void Recordings::createList()
{

    for(int i = 0; i < recordList.size(); i++){
        QFileInfo info = recordList.at(i);
        new QListWidgetItem(info.fileName(),listWidget);

    }



}

void Recordings::sendPlayBackPath(QListWidgetItem *item)
{
    QString temp = recordDir.path() + "/" + item->text();

    const char* recordPath = temp.toStdString().c_str();

    emit sendForPB(recordPath);

    exitScreen();
}

void Recordings::exitScreen()
{
    this->setVisible(false);
    emit outofPIP(false);
}
