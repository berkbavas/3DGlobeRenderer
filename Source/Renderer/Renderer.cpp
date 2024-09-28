#include "Renderer.h"

#include "Util/Logger.h"
#include "Util/Math.h"
#include "Util/Util.h"

#include <QOpenGLFramebufferObject>

EarthRenderer::Renderer::Renderer(QObject* parent)
    : QObject(parent)
{
}

bool EarthRenderer::Renderer::Initialize()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    mEarthShader = new Shader("Earth Shader");
    mEarthShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Earth.vert");
    mEarthShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Earth.frag");
    mEarthShader->Initialize();

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
    mCamera->GetDistance() = 20;
    mCamera->UpdateTransformation();
    mCamera->SetVerticalFov(80.0f);
    mCamera->SetZNear(0.1f);
    mCamera->SetZFar(10000.0f);

    mEarth = new Earth(this);
    mEarth->SetPosition(QVector3D(0, 0, 0));
    mEarth->SetScale(1, 1, 1);
    mEarth->LoadModelData("Resources/Models/Earth.obj");
    mEarth->AddTexture(0, "Resources/Textures/world.topo.bathy.200406.3x5400x2700.jpg");
    mEarth->AddTexture(1, "Resources/HeightMaps/gebco_08_rev_elev_21600x10800.png");

    mSpace = new Space(this);
    mSpace->LoadModelData("Resources/Models/Cube.obj");
    mSpace->LoadTextures("Resources/Textures/Stars", ".jpg");

    LOG_DEBUG("Renderer::Initialize: Application is running...");

    return true;
}

QVector3D EarthRenderer::Renderer::GetMouseWorldPosition(int x, int y)
{
    QVector3D position;
    mMousePositionFramebuffer->bind();
    glReadPixels(x, mMousePositionFramebuffer->height() - y, 1, 1, GL_RGBA, GL_FLOAT, &position);
    mMousePositionFramebuffer->release();
    return position;
}

void EarthRenderer::Renderer::Render(float ifps)
{
    QOpenGLFramebufferObject::bindDefault();
    glViewport(0, 0, mWidth, mHeight);
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderSpace();
    RenderEarth();
    RenderForMousePosition();
}

void EarthRenderer::Renderer::RenderSpace()
{
    mSpaceShader->Bind();
    mSpaceShader->SetUniformValue("view", mEarth->GetTransformation());
    mSpaceShader->SetUniformValue("projection", mCamera->GetProjectionMatrix());
    mSpaceShader->SetUniformValue("skybox", 0);
    mSpaceShader->SetUniformValue("brightness", mSpace->GetBrightness());
    mSpace->Render();
    mSpaceShader->Release();
}

void EarthRenderer::Renderer::RenderEarth()
{
    mEarthShader->Bind();
    mEarthShader->SetUniformValue("M", mEarth->GetTransformation());
    mEarthShader->SetUniformValue("N", mEarth->GetTransformation().normalMatrix());
    mEarthShader->SetUniformValue("VP", mCamera->GetViewProjectionMatrix());
    mEarthShader->SetUniformValue("earth.ambient", mEarth->GetAmbient());
    mEarthShader->SetUniformValue("earth.diffuse", mEarth->GetDiffuse());
    mEarthShader->SetUniformValue("earth.specular", mEarth->GetSpecular());
    mEarthShader->SetUniformValue("earth.shininess", mEarth->GetShininess());
    mEarthShader->SetUniformValue("earth.texture", 0);
    mEarthShader->SetUniformValue("cameraPosition", mCamera->GetPosition());
    mEarthShader->SetUniformValue("sun.direction", mSun->GetDirection());
    mEarthShader->SetUniformValue("sun.color", mSun->GetColor());
    mEarthShader->SetUniformValue("sun.ambient", mSun->GetAmbient());
    mEarthShader->SetUniformValue("sun.diffuse", mSun->GetDiffuse());
    mEarthShader->SetUniformValue("sun.specular", mSun->GetSpecular());
    mEarthShader->SetUniformValue("heightMap", 1);
    mEarth->BindTextures();
    mEarth->Render();
    mEarth->ReleaseTextures();
    mEarthShader->Release();
}

void EarthRenderer::Renderer::RenderForMousePosition()
{
    const auto& vp = mCamera->GetViewProjectionMatrix();
    auto model = mEarth->GetTransformation();

    // We have to scale it little bit in order to be able to rotate the earth
    // while the cursor is not on the earth.
    model.scale(2.0f);

    mMousePositionFramebuffer->bind();
    glViewport(0, 0, mWidth, mHeight);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mMousePositionShader->Bind();
    mMousePositionShader->SetUniformValue("MVP", vp * model);
    mEarth->Render();
    mMousePositionShader->Release();
    mMousePositionFramebuffer->release();
}
void EarthRenderer::Renderer::DrawGui()
{
    if (!ImGui::CollapsingHeader("Earth"))
    {
        ImGui::SliderFloat("Ambient##Earth", &mEarth->GetAmbient_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Diffuse##Earth", &mEarth->GetDiffuse_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Specular##Earth", &mEarth->GetSpecular_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Shininess##Earth", &mEarth->GetShininess_NonConst(), 1.0f, 64.0f, "%.3f");

        if (ImGui::SliderFloat("Scale##Earth", &(mEarth->GetScale()[2]), 1.0f, 10.0f, "%.3f"))
        {
            mEarth->UpdateTransformation();
        }

        ImGui::SliderFloat("Background Brightness##Earth", &mSpace->GetBrightness_NonConst(), 0.0f, 1.0f, "%.3f");
    }

    if (!ImGui::CollapsingHeader("Sun"))
    {
        ImGui::SliderFloat("Ambient##Sun", &mSun->GetAmbient_NonConst(), 0.0f, 2.0f, "%.3f");
        ImGui::SliderFloat("Diffuse##Sun", &mSun->GetDiffuse_NonConst(), 0.0f, 2.0f, "%.3f");
        ImGui::SliderFloat("Specular##Sun", &mSun->GetSpecular_NonConst(), 0.0f, 2.0f, "%.3f");

        auto theta = mSun->GetTheta();
        auto phi = mSun->GetPhi();
        ImGui::SliderFloat("Theta", &theta, -180.0f, 180.0f, "%.1f");
        ImGui::SliderFloat("Phi", &phi, -90.0f, 90.0f, "%.1f");
        mSun->SetDirectionFromThetaPhi(theta, phi);
    }

    if (!ImGui::CollapsingHeader("Camera"))
    {
        if (ImGui::SliderFloat("Tilt##Camera", &mCamera->GetTilt_NonConst(), -89.0f, 89.0f, "%.4f"))
        {
            mCamera->UpdateTransformation();
        }

        if (ImGui::SliderFloat("Distance##Camera", &mCamera->GetDistance(), 10.5f, 50.0f, "%.4f"))
        {
            mCamera->UpdateTransformation();
        }
    }

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Latitude: %.6f, Longitude: %.6f)", mMouseWorldPosition[0], mMouseWorldPosition[1]);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void EarthRenderer::Renderer::Resize(int width, int height)
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

EarthRenderer::Camera* EarthRenderer::Renderer::GetCamera()
{
    return mCamera;
}

EarthRenderer::Earth* EarthRenderer::Renderer::GetEarth()
{
    return mEarth;
}
