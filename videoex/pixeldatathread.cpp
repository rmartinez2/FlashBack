#include "pixeldatathread.h"

pixelDataThread::pixelDataThread(QObject *parent) :
    QThread(parent)
{
}

void pixelDataThread::run()
{

        for(int i = 0; i < height; i++){
            for(int j = 0; j < step; j++){
                temp[step * i + j ] = data[step * i + j ] / 3;
                temp[step * i + j + 1] = data[step * i + j + 1] / 3;
                temp[step * i + j + 2] = data[step * i + j + 2] / 3;
            }
        }


        //this->quit();


}

void pixelDataThread::getArray(uchar *nData)
{
    this->temp = nData;
}

void pixelDataThread::getData(uchar *imgData)
{
    this->data = imgData;
}

void pixelDataThread::getWHS(int height, int width, int step)
{
    this->height = height;
    this->width = width;
    this->step = step;
}
