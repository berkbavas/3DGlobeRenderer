#pragma once

#include "Node/Camera/PersecutorCamera.h"
#include "Node/Globe.h"
#include "Node/Space.h"
#include "Node/Sun.h"
#include "Renderer/Shader.h"
#include "Renderer/TextureLoader.h"

#include <QMouseEvent>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions>

namespace GlobeRenderer
{
    class Renderer : protected QOpenGLFunctions
    {
        DISABLE_COPY(Renderer);

      public:
        Renderer() = default;

        bool Initialize();

        void Render(float ifps);
        void DrawGui(float ifps);
        void Resize(int w, int h);

        void MouseMoved(QMouseEvent* event);

        QVector3D GetMouseWorldPosition(int x, int y);

      private:
        void RenderSpace();
        void RenderGlobe();
        void RenderForMousePosition();

      private:
        Shader* mGlobeShader;
        Shader* mMousePositionShader;
        Shader* mSpaceShader;

        TextureLoader* mTextureLoader;

        DEFINE_MEMBER_PTR(Sun, Sun);
        DEFINE_MEMBER_PTR(PersecutorCamera, Camera);
        DEFINE_MEMBER_PTR(Globe, Globe);
        DEFINE_MEMBER_PTR(Space, Space);

        float mWidth{ INITIAL_WIDTH };
        float mHeight{ INITIAL_HEIGHT };

        QOpenGLFramebufferObjectFormat mMousePositionFramebufferFormat;
        QOpenGLFramebufferObject* mMousePositionFramebuffer{ nullptr };
        QVector4D mMousePositionOnGlobe;

        DEFINE_MEMBER(float, DevicePixelRatio, 1.0f);
    };
}