#ifndef VIDEOPICTURE_H
#define VIDEOPICTURE_H

#include <QtCore>
#include "opencv2/core/core.hpp"

extern "C"{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/imgutils.h"
    #include "libavutil/opt.h"
    #include "libswscale/swscale.h"
    #include "libavformat/avio.h"
    #include "libavutil/avstring.h"
    #include "libavutil/time.h"
    #include "libswresample/swresample.h"
}

using namespace cv;

class VideoPicture
{
public:
    VideoPicture();

    Mat picture;
    QPixmap *pixmap;
    int width, height;
    int allocated;
    double pts;



};

#endif // VIDEOPICTURE_H
