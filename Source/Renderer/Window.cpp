#include "Window.h"

#include "Util/Logger.h"

#include <QDateTime>
#include <QDebug>
#include <QKeyEvent>

EarthRenderer::Window::Window(QWindow* parent)
    : QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent)

{
    // QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    // format.setSamples(4);
    // format.setSwapInterval(1);
    // setFormat(format);

    connect(this, &QOpenGLWindow::frameSwapped, [=]()
            { update(); });
}

void EarthRenderer::Window::initializeGL()
{
    initializeOpenGLFunctions();

    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    mPreviousTime = mCurrentTime;

    QtImGui::initialize(this);

    emit Initialize();
}

void EarthRenderer::Window::resizeGL(int width, int height)
{
    emit Resize(width, height);
}

void EarthRenderer::Window::paintGL()
{
    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    const float ifps = (mCurrentTime - mPreviousTime) * 0.001f;
    mPreviousTime = mCurrentTime;

    emit Update(ifps);
    emit Render(ifps);
}

void EarthRenderer::Window::keyPressEvent(QKeyEvent* event)
{
    emit KeyPressed(event);
}

void EarthRenderer::Window::keyReleaseEvent(QKeyEvent* event)
{
    emit KeyReleased(event);
}

void EarthRenderer::Window::mousePressEvent(QMouseEvent* event)
{
    emit MousePressed(event);
}

void EarthRenderer::Window::mouseReleaseEvent(QMouseEvent* event)
{
    emit MouseReleased(event);
}

void EarthRenderer::Window::mouseMoveEvent(QMouseEvent* event)
{
    emit MouseMoved(event);
}

void EarthRenderer::Window::wheelEvent(QWheelEvent* event)
{
    emit WheelMoved(event);
}
