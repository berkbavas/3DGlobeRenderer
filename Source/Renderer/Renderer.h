#pragma once

#include "Node/Camera.h"
#include "Node/Globe.h"
#include "Node/Space.h"
#include "Node/Sun.h"
#include "Renderer/Shader.h"

#include <QMouseEvent>
#include <QObject>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions>

namespace GlobeRenderer
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

        void MouseMoved(QMouseEvent*);

        QVector3D GetMouseWorldPosition(int x, int y);

        Camera* GetCamera();
        Globe* GetGlobe();

      private:
        void RenderSpace();
        void RenderGlobe();
        void RenderForMousePosition();

      private:
        Shader* mGlobeShader;
        Shader* mMousePositionShader;
        Shader* mSpaceShader;

        Sun* mSun;
        Camera* mCamera;
        Globe* mGlobe;
        Space* mSpace;

        float mWidth{1600};
        float mHeight{900};

        QOpenGLFramebufferObjectFormat mMousePositionFramebufferFormat;
        QOpenGLFramebufferObject* mMousePositionFramebuffer{nullptr};
        QVector4D mMousePositionOnGlobe;

        DEFINE_MEMBER(float, DevicePixelRatio, 1.0f);
    };
}