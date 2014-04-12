#-------------------------------------------------
#
# Project created by QtCreator 2013-11-16T22:20:23
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets widgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = videoex
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mat2qimage.cpp \
    bsdetectionthread.cpp \
    cutratedetectionthread.cpp \
    logodetectionthread.cpp \
    cvmatviewer.cpp \
    playbackthread.cpp \
    mainmenu.cpp \
    sidemenu.cpp \
    videothread.cpp

HEADERS  += widget.h \
    mat2qimage.h \
    bsdetectionthread.h \
    cutratedetectionthread.h \
    logodetectionthread.h \
    cvmatviewer.h \
    playbackthread.h \
    mainmenu.h \
    sidemenu.h \
    videothread.h

FORMS    += widget.ui

CONFIG += mobility \ console
MOBILITY += multimedia

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


INCLUDEPATH += /home/marlon/opencv-2.4.8/include/opencv\\include

LIBS += -L/home/marlon/opencv-2.4.8/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_calib3d \
    -lopencv_contrib \
    -lopencv_gpu \
    -lopencv_ml \
    -lopencv_objdetect \
    -lopencv_video \
    -lopencv_videostab \
    -lopencv_flann \
    -lopencv_features2d \
    -lopencv_legacy \
    -lopencv_nonfree \
    -lopencv_photo \
    -lopencv_stitching \
    -lopencv_superres

INCLUDEPATH += /usr/local/include\\include
LIBS += -LC/usr/local/lib -lavdevice -lavformat -lavcodec -lavutil -lswscale
    #-lavcodec \
    #-lavutil \
    #-lavformat \
    #-lavresample \
    #-lavdevice \
    #-lavfilter \
    #-lswscale

