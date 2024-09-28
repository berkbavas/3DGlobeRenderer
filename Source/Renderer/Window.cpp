#include "Window.h"

#include "Util/Logger.h"

#include <QDateTime>
#include <QDebug>
#include <QKeyEvent>

GlobeRenderer::Window::Window(QWindow* parent)
    : QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent)

{
    // QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    // format.setSamples(4);
    // format.setSwapInterval(1);
    // setFormat(format);

    connect(this, &QOpenGLWindow::frameSwapped, [=]()
            { update(); });
}

void GlobeRenderer::Window::initializeGL()
{
    initializeOpenGLFunctions();

    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    mPreviousTime = mCurrentTime;

    QtImGui::initialize(this);

    emit Initialize();
}

void GlobeRenderer::Window::resizeGL(int width, int height)
{
    emit Resize(width, height);
}

void GlobeRenderer::Window::paintGL()
{
    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    const float ifps = (mCurrentTime - mPreviousTime) * 0.001f;
    mPreviousTime = mCurrentTime;

    emit Update(ifps);
    emit Render(ifps);
}

void GlobeRenderer::Window::keyPressEvent(QKeyEvent* event)
{
    emit KeyPressed(event);
}

void GlobeRenderer::Window::keyReleaseEvent(QKeyEvent* event)
{
    emit KeyReleased(event);
}

void GlobeRenderer::Window::mousePressEvent(QMouseEvent* event)
{
    emit MousePressed(event);
}

void GlobeRenderer::Window::mouseReleaseEvent(QMouseEvent* event)
{
    emit MouseReleased(event);
}

void GlobeRenderer::Window::mouseMoveEvent(QMouseEvent* event)
{
    emit MouseMoved(event);
}

void GlobeRenderer::Window::wheelEvent(QWheelEvent* event)
{
    emit WheelMoved(event);
}
