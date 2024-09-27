#pragma once

#include "Node/Camera.h"
#include "Node/Earth.h"
#include "Node/Sun.h"
#include "Renderer/Shader.h"

#include <QMouseEvent>
#include <QObject>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions>

namespace EarthRenderer
{
    class Controller;
    class ShaderManager;

    struct Mouse
    {
        Qt::MouseButton button = Qt::NoButton;
        float x{0};
        float y{0};
        float w{0};
        float dx{0};
        float dy{0};
        float dw{0};
    };

    class Renderer : public QObject, protected QOpenGLFunctions
    {
        DISABLE_COPY(Renderer);

      public:
        explicit Renderer(QObject* parent = nullptr);
        virtual ~Renderer() = default;
        bool Initialize();

        void Update(float ifps);
        void Render(float ifps);
        void Resize(int w, int h);
        void KeyPressed(QKeyEvent*);
        void KeyReleased(QKeyEvent*);
        void MousePressed(QMouseEvent*);
        void MouseReleased(QMouseEvent*);
        void MouseMoved(QMouseEvent*);
        void WheelMoved(QWheelEvent*);

      private:
        void RenderEarth();
        void RenderForMousePosition();
        void DrawGui();

        void RotateEarth(int x, int y, int prevX, int prevY);
        void RotateCamera(float ifps);
        QVector3D GetMouseWorldPosition(int x, int y);

      private:
        Shader* mEarthShader;
        Shader* mMousePositionShader;

        Sun* mSun;
        Camera* mCamera;
        Earth* mEarth;

        Mouse mMouse;
        bool mRotateEarth{false};
        bool mRotateCamera{false};

        int mWidth{1600};
        int mHeight{900};

        QOpenGLFramebufferObjectFormat mMousePositionFramebufferFormat;
        QOpenGLFramebufferObject* mMousePositionFramebuffer{nullptr};
        QVector4D mMouseWorldPosition;
        QVector4D mMouseWorldPositionPrevious;
    };
}