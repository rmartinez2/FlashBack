#include "frameglwidget.h"

FrameGLWidget::FrameGLWidget(const char* path, QWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers),parent)
{
    mSceneChanged = false;
    mBgColor = QColor::fromRgb(150,150,150);

    mOutH = 0;
    mOutW = 0;

    mImgRatio = 4.0f/3.0f;

    mPosX = 0;
    mPosY = 0;

    vidPath = path;



}


bool FrameGLWidget::showImage(Mat img)
{
}

void FrameGLWidget::playVid()
{

    VideoCapture cap(vidPath);

    //int codec = cap.get(CV_CAP_PROP_FOURCC);

    if(cap.isOpened()){

          Mat Frame;

    for(int i = 0; i < cap.get(CV_CAP_PROP_FRAME_COUNT); i++){

        cap >> Frame;

        if(!Frame.empty()){
            imshow("Playback",Frame);
        }

        waitKey(30);

        }

    }




}

void FrameGLWidget::initializeGL()
{

    makeCurrent();
    qglClearColor(mBgColor.darker());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

}

void FrameGLWidget::resizeGL(int w, int h)
{
    makeCurrent();

    glViewport(0,0,(GLint)w,(GLint)h);


   // glMatrixMode(GL_PROJECTION);
  //  glLoadIdentity();


}

void FrameGLWidget::paintGL()
{
    makeCurrent();

    if(!mSceneChanged)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImage();

    mSceneChanged = false;

}

void FrameGLWidget::updateScene()
{
    if(mSceneChanged && this->isVisible())
        updateGL();
}

void FrameGLWidget::renderImage()
{

}
