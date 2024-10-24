#pragma once

#include "Node/Camera/ArcballCamera.h"
#include "Node/Globe.h"
#include "Node/Space.h"
#include "Node/Sun.h"
#include "Renderer/Quad.h"
#include "Renderer/Shader.h"
#include "Renderer/TextureLoader.h"

#include <QMouseEvent>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QVector>

namespace GlobeRenderer
{
    class Renderer : protected QOpenGLExtraFunctions
    {
        DISABLE_COPY(Renderer);

      public:
        Renderer() = default;

        bool Initialize();

        void Render(float ifps);
        void DrawGui(float ifps);
        void Resize(int w, int h);

        void onMouseMoved(QMouseEvent* event);

        QVector3D GetMouseWorldPosition(int x, int y);

        enum FramebufferType
        {
            DEFAULT,
            TEMP,
            MOUSE_POSITION,
        };

      private:
        void RenderSpace();
        void RenderGlobe();
        void RenderForMousePosition();

      private:
        Shader* mGlobeShader;
        Shader* mMousePositionShader;
        Shader* mSpaceShader;
        Shader* mCombineShader; // Here we output blurred texture to the default framebuffer

        TextureLoader* mTextureLoader;

        DEFINE_MEMBER_PTR(Sun, Sun);
        DEFINE_MEMBER_PTR(ArcballCamera, Camera);
        DEFINE_MEMBER_PTR(Globe, Globe);
        DEFINE_MEMBER_PTR(Space, Space);

        float mWidth{ INITIAL_WIDTH };
        float mHeight{ INITIAL_HEIGHT };

        QVector<QOpenGLFramebufferObjectFormat> mFramebufferFormats;
        QVector<QOpenGLFramebufferObject*> mFramebuffers;

        QVector3D mMousePositionOnGlobe;

        Quad* mQuad;

        QMatrix4x4 mPreviousViewProjectionMatrix;

        DEFINE_MEMBER(float, DevicePixelRatio, 1.0f);

        int mNumberOfSamples{ 2 };
    };
}
