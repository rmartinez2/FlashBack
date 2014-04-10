#include "mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent)
{
    toggle = false;
    this->setVisible(toggle);
}

QSize MainMenu::sizeHint()
{
    return QSize(w,h);
}

void MainMenu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    this->setWindowOpacity(0.80);
    painter.drawRect(this->geometry());


}

void MainMenu::setXYWH(int x, int y, int h, int w)
{
    this->x = x;
    this->y = y;
    this->h = h;
    this->w = w;

    this->setGeometry(x,y,w,h);

}
