#include "widget.h"
#include <QApplication>
#include <QMutex>
#include <QWaitCondition>

#ifdef __MINGW32__
#undef main /* Prevents SDL from overriding main() */
#endif



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    
    return a.exec();
}
