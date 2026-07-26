#include "Renderer.h"

#include "Camera/ArcballCamera.h"

#include <imgui.h>

#include <QImageReader>

GlobeRenderer::Renderer::Renderer(QObject *pParent)
    : QObject(pParent)
{
    QImageReader::setAllocationLimit(0);

    mOpenGLWidget = std::make_unique<OpenGLWidget>(nullptr);

    connect(mOpenGLWidget.get(), &OpenGLWidget::Initialized, this, &Renderer::Initialize);
    connect(mOpenGLWidget.get(), &OpenGLWidget::Resized, this, &Renderer::Resize);
    connect(mOpenGLWidget.get(), &OpenGLWidget::Render, this, &Renderer::Render);

    connect(mOpenGLWidget.get(), &OpenGLWidget::KeyPressed, this, &Renderer::OnKeyPressed);
    connect(mOpenGLWidget.get(), &OpenGLWidget::KeyReleased, this, &Renderer::OnKeyReleased);
    connect(mOpenGLWidget.get(), &OpenGLWidget::MousePressed, this, &Renderer::OnMousePressed);
    connect(mOpenGLWidget.get(), &OpenGLWidget::MouseReleased, this, &Renderer::OnMouseReleased);
    connect(mOpenGLWidget.get(), &OpenGLWidget::MouseMoved, this, &Renderer::OnMouseMoved);
    connect(mOpenGLWidget.get(), &OpenGLWidget::WheelMoved, this, &Renderer::OnWheelMoved);
    connect(mOpenGLWidget.get(), &OpenGLWidget::LeaveEvent, this, &Renderer::OnLeaveEvent);
}

void GlobeRenderer::Renderer::Run()
{
    mOpenGLWidget->showMaximized();
}

void GlobeRenderer::Renderer::Initialize()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);

    mRenderRef = QtImGui::initialize(mOpenGLWidget.get(), false);

    mTextureLoader = std::make_unique<TextureLoader>();

    const auto SpaceTexture = CreateSpaceTexture();
    const auto GlobeTexture = CreateGlobeTexture();

    mSpace = std::make_unique<Space>(SpaceTexture);

    mGlobe = std::make_unique<Globe>(1, 360, 360, GlobeTexture);
    mGlobe->SetPosition(QVector3D(0, 0, 0));
    mGlobe->RotateGlobal(QVector3D(1, 0, 0), -60);
    mGlobe->RotateGlobal(QVector3D(0, 1, 0), 60);
    mGlobe->RotateGlobal(QVector3D(0, 0, 1), 20);

    mSun = std::make_unique<Sun>();

    mTimeSimulator = std::make_unique<TimeSimulator>(mSun.get());

    mCamera = std::make_unique<ArcballCamera>();

    mEventReceivers << mCamera.get();

    mFramebufferFormats[Multisample].setSamples(8);
    mFramebufferFormats[Multisample].setAttachment(QOpenGLFramebufferObject::Attachment::Depth);
    mFramebufferFormats[Multisample].setInternalTextureFormat(GL_RGBA32F);
    mExtraColorAttachments[Multisample] = { GL_RGBA32F, GL_RG16F }; // attachment1=geodetic, attachment2=velocity

    mFramebufferFormats[Singlesample].setSamples(0);
    mFramebufferFormats[Singlesample].setAttachment(QOpenGLFramebufferObject::Attachment::Depth);
    mFramebufferFormats[Singlesample].setInternalTextureFormat(GL_RGBA32F);
    mExtraColorAttachments[Singlesample] = { GL_RGBA32F, GL_RG16F }; // attachment1=geodetic, attachment2=velocity

    for (const auto &[Type, Format] : mFramebufferFormats)
    {
        mFramebuffers[Type] = std::make_unique<Framebuffer>(mWidth, mHeight, Format, mExtraColorAttachments[Type]);
    }

    mQuad = std::make_unique<Quad>();

    mGlobeShader = std::make_unique<Shader>("Globe Shader");
    mGlobeShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Globe.vert");
    mGlobeShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Globe.frag");
    mGlobeShader->Initialize();

    mSpaceShader = std::make_unique<Shader>("Space Shader");
    mSpaceShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Space.vert");
    mSpaceShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Space.frag");
    mSpaceShader->Initialize();

    mScreenShader = std::make_unique<Shader>("Screen Shader");
    mScreenShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Quad.vert");
    mScreenShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Screen.frag");
    mScreenShader->Initialize();
}

void GlobeRenderer::Renderer::Resize(int Width, int Height)
{
    mWidth = Width;
    mHeight = Height;

    if (mCamera)
    {
        mCamera->Resize(Width, Height);
    }

    for (const auto &[Type, Framebuffer] : mFramebuffers)
    {
        Framebuffer->Resize(Width, Height);
    }
}

void GlobeRenderer::Renderer::Render(float Ifps)
{
    mDevicePixelRatio = mOpenGLWidget->devicePixelRatio();
    mTimeElapsed += Ifps;

    mCamera->Update(Ifps);
    mTimeSimulator->Update(Ifps);

    RenderToFramebuffer(mFramebuffers[Multisample].get(), mCamera.get());

    // Blit multisample framebuffer to singlesample framebuffer
    mFramebuffers[Multisample]->BlitDepthTo(mFramebuffers[Singlesample].get());
    mFramebuffers[Multisample]->BlitColorBufferTo(mFramebuffers[Singlesample].get(), GL_COLOR_ATTACHMENT0);
    mFramebuffers[Multisample]->BlitColorBufferTo(mFramebuffers[Singlesample].get(), GL_COLOR_ATTACHMENT1);
    mFramebuffers[Multisample]->BlitColorBufferTo(mFramebuffers[Singlesample].get(), GL_COLOR_ATTACHMENT2);

    QOpenGLFramebufferObject::bindDefault();
    glViewport(0, 0, mWidth * mDevicePixelRatio, mHeight * mDevicePixelRatio);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    mScreenShader->Bind();
    mScreenShader->SetSampler("uColorTexture", 0, mFramebuffers[Singlesample]->GetTexture(0), GL_TEXTURE_2D);
    mScreenShader->SetSampler("uVelocityTexture", 1, mFramebuffers[Singlesample]->GetTexture(2), GL_TEXTURE_2D);
    mScreenShader->SetUniform("uMotionBlurEnabled", static_cast<int>(mMotionBlurEnabled));
    mScreenShader->SetUniform("uMotionBlurSamples", mMotionBlurSamples);
    mScreenShader->SetUniform("uMotionBlurStrength", mMotionBlurStrength);
    mQuad->Render();
    mScreenShader->Unbind();

    RenderImGui();

    // Save current matrices for next frame's velocity computation
    mPrevViewProjectionMatrix = mCamera->GetViewProjectionMatrix();
    mPrevRotationMatrix = mCamera->GetRotationMatrix();
}

void GlobeRenderer::Renderer::OnKeyPressed(QKeyEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
    {
        return;
    }

    for (auto *pReceiver : mEventReceivers)
    {
        if (pReceiver->OnKeyPressed(pEvent))
        {
            return;
        }
    }
}

void GlobeRenderer::Renderer::OnKeyReleased(QKeyEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
    {
        return;
    }

    for (auto *pReceiver : mEventReceivers)
    {
        if (pReceiver->OnKeyReleased(pEvent))
        {
            return;
        }
    }
}

void GlobeRenderer::Renderer::OnMousePressed(QMouseEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    for (auto *pReceiver : mEventReceivers)
    {
        if (pReceiver->OnMousePressed(pEvent))
        {
            return;
        }
    }
}

void GlobeRenderer::Renderer::OnMouseReleased(QMouseEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    for (auto *pReceiver : mEventReceivers)
    {
        pReceiver->OnMouseReleased(pEvent);
    }
}

void GlobeRenderer::Renderer::OnMouseMoved(QMouseEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mMousePosition = GetMouseWorldPosition(pEvent->position().x(), pEvent->position().y());

    for (auto *pReceiver : mEventReceivers)
    {
        if (pReceiver->OnMouseMoved(pEvent))
        {
            return;
        }
    }
}

void GlobeRenderer::Renderer::OnWheelMoved(QWheelEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    for (auto *pReceiver : mEventReceivers)
    {
        if (pReceiver->OnWheelMoved(pEvent))
        {
            return;
        }
    }
}

void GlobeRenderer::Renderer::OnLeaveEvent(QEvent *pEvent)
{
    for (auto *pReceiver : mEventReceivers)
    {
        pReceiver->OnLeaveEvent(pEvent);
    }
}

void GlobeRenderer::Renderer::RenderToFramebuffer(Framebuffer *pFramebuffer, PerspectiveCamera *pCamera)
{
    pFramebuffer->Bind();
    glViewport(0, 0, pFramebuffer->GetWidth(), pFramebuffer->GetHeight());
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    RenderSpace();
    RenderGlobe();

    pFramebuffer->Release();
}

void GlobeRenderer::Renderer::RenderSpace()
{
    mSpaceShader->Bind();
    mSpaceShader->SetUniform("uRotation", mCamera->GetRotationMatrix());
    mSpaceShader->SetUniform("uPrevRotation", mPrevRotationMatrix);
    mSpaceShader->SetUniform("uProjection", mCamera->GetProjectionMatrix());
    mSpace->Render(mSpaceShader.get());
    mSpaceShader->Unbind();
}

void GlobeRenderer::Renderer::RenderGlobe()
{
    mGlobeShader->Bind();
    mGlobeShader->SetUniform("uModelMatrix", mGlobe->GetTransformation());
    mGlobeShader->SetUniform("uNormalMatrix", mGlobe->GetTransformation().normalMatrix());
    mGlobeShader->SetUniform("uVP", mCamera->GetViewProjectionMatrix());
    mGlobeShader->SetUniform("uPrevVP", mPrevViewProjectionMatrix);
    mGlobeShader->SetUniform("uGlobe.Ambient", mGlobe->GetAmbient());
    mGlobeShader->SetUniform("uGlobe.Diffuse", mGlobe->GetDiffuse());
    mGlobeShader->SetUniform("uGlobe.Specular", mGlobe->GetSpecular());
    mGlobeShader->SetUniform("uGlobe.Shininess", mGlobe->GetShininess());
    mGlobeShader->SetUniform("uCameraPosition", mCamera->GetPosition());
    mGlobeShader->SetUniform("uSun.Direction", mSun->GetDirection());
    mGlobeShader->SetUniform("uSun.Color", mSun->GetColor());
    mGlobeShader->SetUniform("uSun.Ambient", mSun->GetAmbient());
    mGlobeShader->SetUniform("uSun.Diffuse", mSun->GetDiffuse());
    mGlobeShader->SetUniform("uSun.Specular", mSun->GetSpecular());
    mGlobe->Render(mGlobeShader.get());
    mGlobeShader->Unbind();
}

GlobeRenderer::Texture GlobeRenderer::Renderer::CreateSpaceTexture() const
{
    Texture Texture;
    Texture.Target = GL_TEXTURE_CUBE_MAP;
    Texture.Id = mTextureLoader->LoadTextureCubeMap("Resources/Textures/MilkyWay", ".png");
    Texture.Name = "uTextureCube";
    Texture.Unit = 0;
    return Texture;
}

GlobeRenderer::Texture GlobeRenderer::Renderer::CreateGlobeTexture() const
{
    Texture Texture;
    Texture.Target = GL_TEXTURE_2D;
    Texture.Id = mTextureLoader->LoadTexture2D("Resources/Textures/world.topo.bathy.200411.3x21600x10800.jpg");
    Texture.Name = "uGlobe.Texture";
    Texture.Unit = 0;
    return Texture;
}

QVector3D GlobeRenderer::Renderer::GetMouseWorldPosition(int X, int Y)
{
    const Framebuffer *pFramebuffer = mFramebuffers[Singlesample].get();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, pFramebuffer->GetHandle());
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    const int FbX = static_cast<int>(X);
    const int FbY = static_cast<int>(mHeight - 1 - Y);
    GLfloat Pixel[4] = {};
    glReadPixels(FbX, FbY, 1, 1, GL_RGBA, GL_FLOAT, Pixel);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    return QVector3D(Pixel[0], Pixel[1], Pixel[2]);
}

void GlobeRenderer::Renderer::RenderImGui()
{
    QOpenGLFramebufferObject::bindDefault();

    QtImGui::newFrame(mRenderRef);

    ImGui::Begin("Debug");

    if (ImGui::CollapsingHeader("Globe"))
    {
        ImGui::SliderFloat("Ambient##Globe", &mGlobe->GetAmbient_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Diffuse##Globe", &mGlobe->GetDiffuse_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Specular##Globe", &mGlobe->GetSpecular_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Shininess##Globe", &mGlobe->GetShininess_NonConst(), 1.0f, 64.0f, "%.3f");
        ImGui::SliderFloat("Background Brightness##Globe", &mSpace->GetBrightness_NonConst(), 0.0f, 1.0f, "%.3f");
    }

    if (ImGui::CollapsingHeader("Sun"))
    {
        ImGui::SliderFloat("Ambient##Sun", &mSun->GetAmbient_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Diffuse##Sun", &mSun->GetDiffuse_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Specular##Sun", &mSun->GetSpecular_NonConst(), 0.0f, 1.0f, "%.3f");

        float Theta = mSun->GetTheta();
        float Phi = mSun->GetPhi();
        ImGui::SliderFloat("Theta", &Theta, -179.0f, 179.0f, "%.1f");
        ImGui::SliderFloat("Phi", &Phi, -89.0f, 89.0f, "%.1f");
        mSun->SetDirectionFromThetaPhi(Theta, Phi);
    }

    if (ImGui::CollapsingHeader("Time Simulation"))
    {
        ImGui::Checkbox("Enable Time Simulation##Time", &mTimeSimulator->GetEnabled_NonConst());
        if (mTimeSimulator->GetEnabled())
        {
            ImGui::SliderFloat("Time Speed##Time", &mTimeSimulator->GetTimeSpeed_NonConst(), 1.0f, 3600.0f, "%.0fx");

            QDateTime DateTime = mTimeSimulator->GetDateTime();
            ImGui::Text("Simulated Time: %s", DateTime.toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str());

            if (ImGui::Button("Reset to Now"))
            {
                mTimeSimulator->ResetToCurrentTime();
            }
        }
    }

    if (ImGui::CollapsingHeader("Motion Blur"))
    {
        ImGui::Checkbox("Enabled##MotionBlur", &mMotionBlurEnabled);
        if (mMotionBlurEnabled)
        {
            ImGui::SliderInt("Samples##MotionBlur", &mMotionBlurSamples, 2, 64);
            ImGui::SliderFloat("Strength##MotionBlur", &mMotionBlurStrength, 0.0f, 4.0f, "%.2f");
        }
    }

    if (ImGui::CollapsingHeader("Stats", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Latitude: %.6f, Longitude: %.6f)", mMousePosition[0], mMousePosition[1]);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    ImGui::End();

    ImGui::Render();
    QtImGui::render(mRenderRef);
}
