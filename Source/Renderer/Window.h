#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctionsPrivate>
#include <QOpenGLWindow>
#include <QSharedPointer>
#include <QtImGui.h>
#include <imgui.h>

namespace GlobeRenderer
{
    class Window : public QOpenGLWindow, protected QOpenGLExtraFunctions
    {
        Q_OBJECT
      public:
        Window(QWindow* parent = nullptr);

      private:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
        void keyPressEvent(QKeyEvent*) override;
        void keyReleaseEvent(QKeyEvent*) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseReleaseEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void wheelEvent(QWheelEvent*) override;

      signals:
        void Initialize();
        void Resize(int w, int h);
        void Update(float ifps);
        void Render(float ifps);
        void KeyPressed(QKeyEvent*);
        void KeyReleased(QKeyEvent*);
        void MousePressed(QMouseEvent*);
        void MouseReleased(QMouseEvent*);
        void MouseMoved(QMouseEvent*);
        void WheelMoved(QWheelEvent*);

      private:
        long long mPreviousTime;
        long long mCurrentTime;
    };

    using WindowPtr = QSharedPointer<Window>;
}
