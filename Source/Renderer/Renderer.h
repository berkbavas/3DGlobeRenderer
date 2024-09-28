#pragma once

#include "Node/Camera.h"
#include "Node/Earth.h"
#include "Node/Space.h"
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

    class Renderer : public QObject, protected QOpenGLFunctions
    {
        DISABLE_COPY(Renderer);

      public:
        explicit Renderer(QObject* parent = nullptr);
        virtual ~Renderer() = default;
        bool Initialize();

        void Render(float ifps);
        void DrawGui();

        void Resize(int w, int h);

        QVector3D GetMouseWorldPosition(int x, int y);

        Camera* GetCamera();
        Earth* GetEarth();

      private:
        void RenderSpace();
        void RenderEarth();
        void RenderForMousePosition();

        void RotateEarth(int x, int y, int prevX, int prevY);
        void UpdateCamera(float ifps);

      private:
        Shader* mEarthShader;
        Shader* mMousePositionShader;
        Shader* mSpaceShader;

        Sun* mSun;
        Camera* mCamera;
        Earth* mEarth;
        Space* mSpace;

        float mWidth{1600};
        float mHeight{900};

        QOpenGLFramebufferObjectFormat mMousePositionFramebufferFormat;
        QOpenGLFramebufferObject* mMousePositionFramebuffer{nullptr};
        QVector4D mMouseWorldPosition;
        QVector4D mMouseWorldPositionPrevious;
    };
}