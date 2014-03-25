#include "cvmatviewer.h"

cvMatViewer::cvMatViewer(QWidget *parent) :
    QWidget(parent)
{
}

QSize cvMatViewer::sizeHint() const
{
    return qimg.size();
}

QSize cvMatViewer::minimumSizeHint() const
{
    return qimg.size();
}


void cvMatViewer::showImg(Mat &img)
{

    switch(img.type()){

    case CV_8UC1:
        cvtColor(img, mat,CV_GRAY2RGB);
        break;

    case CV_8UC3:
        cvtColor(img, mat,CV_BGR2RGB);
        break;

    }

    assert(img.isContinuous());

    qimg = QImage(mat.data, mat.cols, mat.rows, mat.cols*3, QImage::Format_RGB888);

    this->setFixedSize(img.cols,img.rows);

    repaint();

}

void cvMatViewer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), qimg);
    painter.end();
}
