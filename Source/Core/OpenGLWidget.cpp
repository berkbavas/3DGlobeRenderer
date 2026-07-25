#include "OpenGLWidget.h"

#include "Util/Logger.h"
#include "Util/Util.h"

#include <QDateTime>

GlobeRenderer::OpenGLWidget::OpenGLWidget(QWidget *pParent)
    : QOpenGLWidget(pParent)
{
    connect(this, &OpenGLWidget::frameSwapped, this, [this]() { update(); });
    setMouseTracking(true);
}

void GlobeRenderer::OpenGLWidget::initializeGL()
{
    qDebug() << "OpenGLWidget::initializeGL: QSurfaceFormat:" << QSurfaceFormat::defaultFormat();

    initializeOpenGLFunctions(); // Initialize OpenGL functions for the current context

    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    mPreviousTime = mCurrentTime;

    emit Initialized();

    CheckGLError();

    qDebug() << "OpenGLWidget::initializeGL: OpenGL version:" << reinterpret_cast<const char *>(glGetString(GL_VERSION));
}

void GlobeRenderer::OpenGLWidget::resizeGL(int Width, int Height)
{
    emit Resized(Width, Height);

    CheckGLError();
}

void GlobeRenderer::OpenGLWidget::paintGL()
{
    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    const float Ifps = (mCurrentTime - mPreviousTime) * 0.001f;
    mPreviousTime = mCurrentTime;

    emit Render(Ifps);

    CheckGLError();
}

void GlobeRenderer::OpenGLWidget::keyPressEvent(QKeyEvent *pEvent)
{
    emit KeyPressed(pEvent);
}

void GlobeRenderer::OpenGLWidget::keyReleaseEvent(QKeyEvent *pEvent)
{
    emit KeyReleased(pEvent);
}

void GlobeRenderer::OpenGLWidget::mousePressEvent(QMouseEvent *pEvent)
{
    emit MousePressed(pEvent);
}

void GlobeRenderer::OpenGLWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    emit MouseReleased(pEvent);
}

void GlobeRenderer::OpenGLWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    emit MouseMoved(pEvent);
}

void GlobeRenderer::OpenGLWidget::wheelEvent(QWheelEvent *pEvent)
{
    emit WheelMoved(pEvent);
}

void GlobeRenderer::OpenGLWidget::closeEvent(QCloseEvent *pEvent)
{
    QOpenGLWidget::closeEvent(pEvent);

    emit WindowClosed();
}

void GlobeRenderer::OpenGLWidget::leaveEvent(QEvent *pEvent)
{
    emit LeaveEvent(pEvent);
}

void GlobeRenderer::OpenGLWidget::CheckGLError()
{
    GLenum Error;
    while ((Error = glGetError()) != GL_NO_ERROR) // Loop until all OpenGL errors are retrieved
    {
        GR_EXIT_FAILURE("OpenGLWidget::CheckGLError: OpenGL error detected: '{}'", GlobeRenderer::Util::GetGlErrorString(Error));
    }
}
