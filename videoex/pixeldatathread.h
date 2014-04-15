#ifndef PIXELDATATHREAD_H
#define PIXELDATATHREAD_H

#include <QThread>

class pixelDataThread : public QThread
{
    Q_OBJECT
public:
    explicit pixelDataThread(QObject *parent = 0);

    uchar* temp;
    uchar* data;

    int height, width,step;


    void run();
    
signals:

    
public slots:
    void getArray(uchar* nData);
    void getData(uchar* imgData);
    void getWHS(int height, int width, int step);
    
};

#endif // PIXELDATATHREAD_H
