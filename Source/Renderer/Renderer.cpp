#include "Renderer.h"

#include "Core/Constants.h"
#include "Util/Logger.h"
#include "Util/Math.h"
#include "Util/Util.h"

#include <QOpenGLFramebufferObject>
#include <QtImGui.h>
#include <imgui.h>

bool GlobeRenderer::Renderer::Initialize()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    mGlobeShader = new Shader("Globe Shader");
    mGlobeShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Basic.vert");
    mGlobeShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Basic.frag");
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

    mMousePositionFramebuffer = new QOpenGLFramebufferObject(mWidth, mHeight, mMousePositionFramebufferFormat);

    mSun = new Sun;

    mTextureLoader = new TextureLoader;

    Texture texture;
    texture.target = GL_TEXTURE_2D;
    texture.id = mTextureLoader->LoadTexture2D("Resources/Textures/world.topo.bathy.200411.3x21600x10800.jpg");
    texture.name = "globe.texture";
    texture.unit = 0;

    mGlobe = new Globe(1, 128, 128);
    mGlobe->SetPosition(QVector3D(0, 0, 0));
    mGlobe->RotateGlobal(QVector3D(1, 0, 0), -60);
    mGlobe->RotateGlobal(QVector3D(0, 1, 0), 60);
    mGlobe->RotateGlobal(QVector3D(0, 0, 1), 20);
    mGlobe->SetTexture(texture);
    mGlobe->Construct();

    mCamera = new ArcballCamera;
    mCamera->SetPosition(0, 0, 2.0f);
    mCamera->UpdateTransformation();
    mCamera->SetVerticalFov(60.0f);
    mCamera->SetZNear(0.1f);
    mCamera->SetZFar(10000.0f);

    texture.target = GL_TEXTURE_CUBE_MAP;
    texture.id = mTextureLoader->LoadTextureCubeMap("Resources/Textures/MilkyWay", ".png");
    texture.name = "skybox";
    texture.unit = 0;

    mSpace = new Space;
    mSpace->SetTexture(texture);
    mSpace->Construct();

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
    mCamera->SetDevicePixelRatio(mDevicePixelRatio);
    mCamera->Update(ifps);

    QOpenGLFramebufferObject::bindDefault();
    glViewport(0, 0, mWidth, mHeight);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderSpace();
    RenderGlobe();
    RenderForMousePosition();
}

void GlobeRenderer::Renderer::RenderSpace()
{
    mSpaceShader->Bind();
    mSpaceShader->SetUniformValue("rotation", mCamera->GetRotationMatrix());
    mSpaceShader->SetUniformValue("projection", mCamera->GetProjectionMatrix());
    mSpaceShader->SetUniformValue(mSpace->GetTexture().name, mSpace->GetTexture().unit);
    mSpaceShader->SetUniformValue("brightness", mSpace->GetBrightness());
    mSpace->Render();
    mSpaceShader->Release();
}

void GlobeRenderer::Renderer::RenderGlobe()
{
    mGlobeShader->Bind();
    mGlobeShader->SetUniformValue("modelMatrix", mGlobe->GetTransformation());
    mGlobeShader->SetUniformValue("normalMatrix", mGlobe->GetTransformation().normalMatrix());
    mGlobeShader->SetUniformValue("viewProjectionMatrix", mCamera->GetViewProjectionMatrix());
    mGlobeShader->SetUniformValue("globe.ambient", mGlobe->GetAmbient());
    mGlobeShader->SetUniformValue("globe.diffuse", mGlobe->GetDiffuse());
    mGlobeShader->SetUniformValue("globe.specular", mGlobe->GetSpecular());
    mGlobeShader->SetUniformValue("globe.shininess", mGlobe->GetShininess());
    mGlobeShader->SetUniformValue(mGlobe->GetTexture().name, mGlobe->GetTexture().unit);
    mGlobeShader->SetUniformValue("cameraPosition", mCamera->GetPosition());
    mGlobeShader->SetUniformValue("sun.direction", mSun->GetDirection());
    mGlobeShader->SetUniformValue("sun.color", mSun->GetColor());
    mGlobeShader->SetUniformValue("sun.ambient", mSun->GetAmbient());
    mGlobeShader->SetUniformValue("sun.diffuse", mSun->GetDiffuse());
    mGlobeShader->SetUniformValue("sun.specular", mSun->GetSpecular());
    mGlobe->Render();
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

void GlobeRenderer::Renderer::DrawGui(float ifps)
{
    if (ImGui::CollapsingHeader("Globe"))
    {
        ImGui::SliderFloat("Ambient##Globe", &mGlobe->GetAmbient_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Diffuse##Globe", &mGlobe->GetDiffuse_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Specular##Globe", &mGlobe->GetSpecular_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Shininess##Globe", &mGlobe->GetShininess_NonConst(), 1.0f, 64.0f, "%.3f");

        if (ImGui::SliderFloat("Scale##Globe", &mGlobe->GetAllScale_NonConst(), 1.0f, 4.0f, "%.4f"))
        {
            mGlobe->UpdateAllScale();
        }

        ImGui::SliderFloat("Background Brightness##Globe", &mSpace->GetBrightness_NonConst(), 0.0f, 1.0f, "%.3f");
    }

    if (ImGui::CollapsingHeader("Sun"))
    {
        ImGui::SliderFloat("Ambient##Sun", &mSun->GetAmbient_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Diffuse##Sun", &mSun->GetDiffuse_NonConst(), 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Specular##Sun", &mSun->GetSpecular_NonConst(), 0.0f, 1.0f, "%.3f");

        auto theta = mSun->GetTheta();
        auto phi = mSun->GetPhi();
        ImGui::SliderFloat("Theta", &theta, -179.0f, 179.0f, "%.1f");
        ImGui::SliderFloat("Phi", &phi, -89.0f, 89.0f, "%.1f");
        mSun->SetDirectionFromThetaPhi(theta, phi);
    }

    if (ImGui::CollapsingHeader("Camera"))
    {
        if (ImGui::SliderFloat("Distance##Camera", &mCamera->GetDistance_NonConst(), mCamera->GetMinimumDistance(), mCamera->GetMaximumDistance(), "%.4f"))
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

    mCamera->Resize(mWidth, mHeight);

    if (mMousePositionFramebuffer)
    {
        delete mMousePositionFramebuffer;
        mMousePositionFramebuffer = nullptr;
    }

    mMousePositionFramebuffer = new QOpenGLFramebufferObject(mWidth, mHeight, mMousePositionFramebufferFormat);
}

void GlobeRenderer::Renderer::onMouseMoved(QMouseEvent* event)
{
    const int x = mDevicePixelRatio * event->position().x();
    const int y = mDevicePixelRatio * event->position().y();
    mMousePositionOnGlobe = GetMouseWorldPosition(x, y);
}