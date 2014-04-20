#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T10:01:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia multimediawidgets opengl

TARGET = test2
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mat2qimage.cpp \
    videothread.cpp \
    bsdetectionthread.cpp \
    cutratedetectionthread.cpp \
    logodetectionthread.cpp \
    cvmatviewer.cpp \
    playbackthread.cpp \
    mainmenu.cpp \
    sidemenu.cpp \
    slidervisual.cpp \
    notificationwidget.cpp \
    seekerbar.cpp \
    seekerbarticker.cpp \
    timeitem.cpp \
    pixeldatathread.cpp \
    recordingoptions.cpp \
    recordings.cpp \
    recordplayback.cpp \







HEADERS  += widget.h \
    mat2qimage.h \
    videothread.h \
    bsdetectionthread.h \
    cutratedetectionthread.h \
    logodetectionthread.h \
    cvmatviewer.h \
    playbackthread.h \
    mainmenu.h \
    sidemenu.h \
    slidervisual.h \
    notificationwidget.h \
    seekerbar.h \
    seekerbarticker.h \
    timeitem.h \
    pixeldatathread.h \
    recordingoptions.h \
    recordings.h \
    recordplayback.h

FORMS    += widget.ui

CONFIG += mobility \ console

#MOBILITY += multimedia

QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS

#add QtMEL Library

#INCLUDEPATH += C:\\Users\\Rene\\Downloads\\QtMEL_Library\\include
#LIBS += -LC:\\Users\\Rene\\Downloads\\QtMEL_Library\\lib
#CONFIG(debug,debug|release){

#LIBS += -lqtmeld1

#}
#else{
#LIBS += -lqtmel1
#}


#INCLUDEPATH += C:\\openCV_build_debug\\install\\include

#LIBS += -LC:\\openCV_build_debug\\install\\lib\
#    -llibopencv_core246 \
#    -llibopencv_highgui246 \
#    -llibopencv_imgproc246 \
#    -llibopencv_calib3d246 \
#    -llibopencv_contrib246 \
#    -llibopencv_gpu246 \
#    -llibopencv_ml246 \
#    -llibopencv_objdetect \
#    -llibopencv_video246 \
#    -llibopencv_videostab246 \
#    -llibopencv_flann246 \
#    -llibopencv_features2d246 \
#    -llibopencv_legacy246 \
#    -llibopencv_nonfree246 \
#    -llibopencv_photo246 \
#    -llibopencv_stitching246 \
#    -llibopencv_superres246


INCLUDEPATH += C:\OpenCV6.0\opencv\build\include

LIBS += -LC:\OpenCV6.0\opencv\build\x86\mingw\lib\
-llibopencv_core246 \
    -llibopencv_highgui246 \
    -llibopencv_imgproc246 \
    -llibopencv_calib3d246 \
    -llibopencv_contrib246 \
    -llibopencv_gpu246 \
    -llibopencv_ml246 \
    -llibopencv_objdetect \
    -llibopencv_video246 \
    -llibopencv_videostab246 \
    -llibopencv_flann246 \
    -llibopencv_features2d246 \
    -llibopencv_legacy246 \
    -llibopencv_nonfree246 \
    -llibopencv_photo246 \
    -llibopencv_stitching246 \
    -llibopencv_superres246





INCLUDEPATH += C:\\Users\\Rene\\Downloads\\ffmpeg-20140110-git-d9481dc-win32-dev\\ffmpeg-20140110-git-d9481dc-win32-dev\\include

LIBS += -LC:\\Users\\Rene\\Downloads\\ffmpeg-20140110-git-d9481dc-win32-dev\\ffmpeg-20140110-git-d9481dc-win32-dev\\lib \
    -lavcodec \
    -lavformat \
    -lavutil \
    -lswresample \
    -lpostproc \
    -lswscale \
    -lavdevice \
    -lavfilter




OTHER_FILES += \
    avutil-52.dll \
    avformat-55.dll \
    avfilter-4.dll \
    avdevice-55.dll \
    avcodec-55.dll \
