#include "Window.h"
#include "Controller.h"

#include <QDateTime>
#include <QKeyEvent>

#include <QDebug>

Window::Window(QWindow *parent)
    : QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent)

{
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);
    format.setSwapInterval(1);
    setFormat(format);

    connect(this, &QOpenGLWindow::frameSwapped, this, [=]() { update(); });
}

void Window::initializeGL()
{
    initializeOpenGLFunctions();

    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    mPreviousTime = mCurrentTime;

    QtImGui::initialize(this);

    mController = new Controller;
    mController->setWindow(this);
    mController->init();
}

void Window::resizeGL(int w, int h)
{
    glViewport(0, 0, width(), height());

    mController->resize(w, h);
}

void Window::paintGL()
{
    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    float ifps = (mCurrentTime - mPreviousTime) * 0.001f;
    mPreviousTime = mCurrentTime;

    mController->render(ifps);
}

void Window::keyPressEvent(QKeyEvent *event)
{
    mController->keyPressed(event);
}

void Window::keyReleaseEvent(QKeyEvent *event)
{
    mController->keyReleased(event);
}

void Window::mousePressEvent(QMouseEvent *event)
{
    mController->mousePressed(event);
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
    mController->mouseReleased(event);
}

void Window::mouseMoveEvent(QMouseEvent *event)
{
    mController->mouseMoved(event);
}

void Window::wheelEvent(QWheelEvent *event)
{
    mController->wheelMoved(event);
}
