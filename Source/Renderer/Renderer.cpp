#include "Renderer.h"

#include "Core/Constants.h"
#include "Util/Logger.h"
#include "Util/Math.h"
#include "Util/Util.h"

#include <QOpenGLFramebufferObject>

GlobeRenderer::Renderer::Renderer(QObject* parent)
    : QObject(parent)
{
}

bool GlobeRenderer::Renderer::Initialize()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    mGlobeShader = new Shader("Globe Shader");
    mGlobeShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Globe.vert");
    mGlobeShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Globe.frag");
    mGlobeShader->Initialize();

    mMousePositionShader = new Shader("Mouse Position Shader");
    mMousePositionShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/MousePosition.vert");
    mMousePositionShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/MousePosition.frag");
    mMousePositionShader->Initialize();

    mSpaceShader = new Shader("Space Cube Map Shader");
    mSpaceShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Space.vert");
    mSpaceShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Space.frag");
    mSpaceShader->Initialize();

    mMousePositionFramebufferFormat.setSamples(0);
    mMousePositionFramebufferFormat.setAttachment(QOpenGLFramebufferObject::Attachment::Depth);
    mMousePositionFramebufferFormat.setInternalTextureFormat(GL_RGBA32F);

    mMousePositionFramebuffer = new QOpenGLFramebufferObject(1600, 900, mMousePositionFramebufferFormat);

    mSun = new Sun(this);

    mCamera = new Camera(this);
    mCamera->GetDistance() = 3.5f;
    mCamera->UpdateTransformation();
    mCamera->SetVerticalFov(60.0f);
    mCamera->SetZNear(0.1f);
    mCamera->SetZFar(10000.0f);

    mGlobe = new Globe(this);
    mGlobe->SetPosition(QVector3D(0, 0, 0));
    mGlobe->AddTexture(0, "Resources/Textures/world.topo.bathy.200411.3x21600x10800.jpg");
    // mGlobe->AddTexture(1, "Resources/HeightMaps/gebco_08_rev_elev_21600x10800.png");

    mGlobe->Rotate(mCamera->GetRotation() * QVector3D(1, 0, 0), -60);
    mGlobe->Rotate(mCamera->GetRotation() * QVector3D(0, 1, 0), 60);
    mGlobe->Rotate(mCamera->GetRotation() * QVector3D(0, 0, 1), 20);

    mSpace = new Space(this);
    mSpace->LoadModelData("Resources/Models/Cube.obj");
    mSpace->LoadTextures("Resources/Textures/MilkyWay", ".png");

    LOG_DEBUG("Renderer::Initialize: Application is running...");

    return true;
}

QVector3D GlobeRenderer::Renderer::GetMouseWorldPosition(int x, int y)
{
    QVector3D position;
    mMousePositionFramebuffer->bind();
    glReadPixels(x, mMousePositionFramebuffer->height() - y, 1, 1, GL_RGBA, GL_FLOAT, &position);
    mMousePositionFramebuffer->release();
    return position;
}

void GlobeRenderer::Renderer::Render(float ifps)
{
    QOpenGLFramebufferObject::bindDefault();
    glViewport(0, 0, mWidth, mHeight);
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderSpace();
    RenderGlobe();
    RenderForMousePosition();
}

void GlobeRenderer::Renderer::RenderSpace()
{
    mSpaceShader->Bind();
    mSpaceShader->SetUniformValue("view", mGlobe->GetTransformation());
    mSpaceShader->SetUniformValue("projection", mCamera->GetProjectionMatrix());
    mSpaceShader->SetUniformValue("skybox", 0);
    mSpaceShader->SetUniformValue("brightness", mSpace->GetBrightness());
    mSpace->Render();
    mSpaceShader->Release();
}

void GlobeRenderer::Renderer::RenderGlobe()
{
    mGlobeShader->Bind();
    mGlobeShader->SetUniformValue("M", mGlobe->GetTransformation());
    mGlobeShader->SetUniformValue("N", mGlobe->GetTransformation().normalMatrix());
    mGlobeShader->SetUniformValue("VP", mCamera->GetViewProjectionMatrix());
    mGlobeShader->SetUniformValue("globe.ambient", mGlobe->GetAmbient());
    mGlobeShader->SetUniformValue("globe.diffuse", mGlobe->GetDiffuse());
    mGlobeShader->SetUniformValue("globe.specular", mGlobe->GetSpecular());
    mGlobeShader->SetUniformValue("globe.shininess", mGlobe->GetShininess());
    mGlobeShader->SetUniformValue("globe.texture", 0);
    // mGlobeShader->SetUniformValue("heightMap", 1);
    mGlobeShader->SetUniformValue("cameraPosition", mCamera->GetPosition());
    mGlobeShader->SetUniformValue("sun.direction", mSun->GetDirection());
    mGlobeShader->SetUniformValue("sun.color", mSun->GetColor());
    mGlobeShader->SetUniformValue("sun.ambient", mSun->GetAmbient());
    mGlobeShader->SetUniformValue("sun.diffuse", mSun->GetDiffuse());
    mGlobeShader->SetUniformValue("sun.specular", mSun->GetSpecular());
    mGlobe->BindTextures();
    mGlobe->Render();
    mGlobe->ReleaseTextures();
    mGlobeShader->Release();
}

void GlobeRenderer::Renderer::RenderForMousePosition()
{
    mMousePositionFramebuffer->bind();
    glViewport(0, 0, mWidth, mHeight);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mMousePositionShader->Bind();
    mMousePositionShader->SetUniformValue("MVP", mCamera->GetViewProjectionMatrix() * mGlobe->GetTransformation());
    mGlobe->Render();
    mMousePositionShader->Release();
    mMousePositionFramebuffer->release();
}

void GlobeRenderer::Renderer::DrawGui()
{
    if (!ImGui::CollapsingHeader("Globe"))
    {
        ImGui::SliderFloat("Ambient##Globe", &mGlobe->GetAmbient_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Diffuse##Globe", &mGlobe->GetDiffuse_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Specular##Globe", &mGlobe->GetSpecular_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Shininess##Globe", &mGlobe->GetShininess_NonConst(), 1.0f, 64.0f, "%.3f");

        if (ImGui::SliderFloat("Scale##Globe", &(mGlobe->GetScale()[2]), 1.0f, 10.0f, "%.3f"))
        {
            mGlobe->UpdateTransformation();
        }

        ImGui::SliderFloat("Background Brightness##Globe", &mSpace->GetBrightness_NonConst(), 0.0f, 1.0f, "%.3f");
    }

    if (!ImGui::CollapsingHeader("Sun"))
    {
        ImGui::SliderFloat("Ambient##Sun", &mSun->GetAmbient_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Diffuse##Sun", &mSun->GetDiffuse_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Specular##Sun", &mSun->GetSpecular_NonConst(), 0.0f, 1.0f, "%.3f");

        auto theta = mSun->GetTheta();
        auto phi = mSun->GetPhi();
        ImGui::SliderFloat("Theta", &theta, -180.0f, 180.0f, "%.1f");
        ImGui::SliderFloat("Phi", &phi, -90.0f, 90.0f, "%.1f");
        mSun->SetDirectionFromThetaPhi(theta, phi);
    }

    if (!ImGui::CollapsingHeader("Camera"))
    {
        if (ImGui::SliderFloat("Tilt##Camera", &mCamera->GetTilt_NonConst(), GR_MIN_TILT_ANGLE, GR_MAX_TILT_ANGLE, "%.4f"))
        {
            mCamera->UpdateTransformation();
        }

        if (ImGui::SliderFloat("Distance##Camera", &mCamera->GetDistance(), GR_MIN_CAM_DISTANCE, GR_MAX_CAM_DISTANCE, "%.4f"))
        {
            mCamera->UpdateTransformation();
        }
    }

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Latitude: %.6f, Longitude: %.6f)", mMousePositionOnGlobe[0], mMousePositionOnGlobe[1]);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void GlobeRenderer::Renderer::Resize(int width, int height)
{
    mWidth = width;
    mHeight = height;

    glViewport(0, 0, mWidth, mHeight);

    mCamera->Resize(mWidth, mHeight);

    if (mMousePositionFramebuffer)
    {
        delete mMousePositionFramebuffer;
        mMousePositionFramebuffer = nullptr;
    }

    mMousePositionFramebuffer = new QOpenGLFramebufferObject(mWidth, mHeight, mMousePositionFramebufferFormat);
}

void GlobeRenderer::Renderer::MouseMoved(QMouseEvent* event)
{
    const int x = mDevicePixelRatio * event->pos().x();
    const int y = mDevicePixelRatio * event->pos().y();
    mMousePositionOnGlobe = GetMouseWorldPosition(x, y);
}

GlobeRenderer::Camera* GlobeRenderer::Renderer::GetCamera()
{
    return mCamera;
}

GlobeRenderer::Globe* GlobeRenderer::Renderer::GetGlobe()
{
    return mGlobe;
}
