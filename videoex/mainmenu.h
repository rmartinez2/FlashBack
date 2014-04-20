#ifndef MAINMENU_H
#define MAINMENU_H

//#if QT_VERSION >= 0x050000
//#include <QtWidgets>
//#else
//#include <QtGui>
//#endif

#include <QtGui>

#include <QtCore>
#include <QPainter>

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = 0);

  QSize sizeHint();
  void paintEvent(QPaintEvent *event);

  bool toggle;
  int w,h;
  int x,y;
    
signals:
    
public slots:
  void setXYWH(int x, int y, int h, int w);

protected:

    
};

#endif // MAINMENU_H
