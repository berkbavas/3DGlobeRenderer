#pragma once

#include "Camera/PerspectiveCamera.h"
#include "Core/Enums.h"
#include "Core/EventReceiver.h"
#include "Core/Framebuffer.h"
#include "Core/OpenGLWidget.h"
#include "Core/Quad.h"
#include "Core/Shader.h"
#include "Core/TimeSimulator.h"
#include "Node/Globe.h"
#include "Node/Space.h"
#include "Node/Sun.h"
#include "Util/TextureLoader.h"

#include <map>

#include <QObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QVector>
#include <QtImGui.h>

namespace GlobeRenderer
{
    class Renderer : public QObject, protected QOpenGLFunctions_4_5_Core
    {
      public:
        explicit Renderer(QObject* pParent = nullptr);

        void Run();

      private:
        void Initialize();
        void Resize(int Width, int Height);
        void Render(float Ifps);

        void OnKeyPressed(QKeyEvent* pEvent);
        void OnKeyReleased(QKeyEvent* pEvent);
        void OnMousePressed(QMouseEvent* pEvent);
        void OnMouseReleased(QMouseEvent* pEvent);
        void OnMouseMoved(QMouseEvent* pEvent);
        void OnWheelMoved(QWheelEvent* pEvent);
        void OnLeaveEvent(QEvent* pEvent);

        void RenderToFramebuffer(Framebuffer* pFramebuffer, PerspectiveCamera* pCamera);
        void RenderSpace();
        void RenderGlobe();
        Texture CreateSpaceTexture() const;
        Texture CreateGlobeTexture() const;
        QVector3D GetMouseWorldPosition(int X, int Y);
        void RenderImGui();

        QVector<EventReceiver*> mEventReceivers;

        OpenGLWidgetPtr mOpenGLWidget{ nullptr };

        // Nodes
        PerspectiveCameraPtr mCamera{ nullptr };
        SunPtr mSun{ nullptr };

        // Renderables
        QuadPtr mQuad{ nullptr };
        GlobePtr mGlobe{ nullptr };
        SpacePtr mSpace{ nullptr };

        // Shaders
        ShaderPtr mGlobeShader{ nullptr };
        ShaderPtr mSpaceShader{ nullptr };
        ShaderPtr mScreenShader{ nullptr };

        // Framebuffers
        std::map<FramebufferType, QOpenGLFramebufferObjectFormat> mFramebufferFormats;
        std::map<FramebufferType, QVector<GLuint>> mExtraColorAttachments;
        std::map<FramebufferType, FramebufferPtr> mFramebuffers;

        // Util
        TextureLoaderPtr mTextureLoader{ nullptr };

        // State
        float mTimeElapsed{ 0.0f };
        QVector3D mMousePosition{ 0.0f, 0.0f, 0.0f };
        QtImGui::RenderRef mRenderRef{ nullptr };
        int mWidth{ 1 };
        int mHeight{ 1 };
        float mDevicePixelRatio{ 1.0f };

        // Managers
        TimeSimulatorPtr mTimeSimulator{ nullptr };
    };
}