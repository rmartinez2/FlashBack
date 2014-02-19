#ifndef MAT2QIMAGE_H
#define MAT2QIMAGE_H

#pragma once
#include <QImage>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;



QImage Mat2QImage(cv::Mat const&src);
Mat QImage2Mat(QImage img);

#endif // MAT2QIMAGE_H
