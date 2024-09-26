#include "Renderer.h"

#include "Util/Logger.h"
#include "Util/Math.h"
#include "Util/Misc.h"

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

    mMousePositionFramebufferFormat.setSamples(0);
    mMousePositionFramebufferFormat.setAttachment(QOpenGLFramebufferObject::Attachment::Depth);
    mMousePositionFramebufferFormat.setInternalTextureFormat(GL_RGBA32F);

    mMousePositionFramebuffer = new QOpenGLFramebufferObject(mWidth, mHeight, mMousePositionFramebufferFormat);

    mMousePositionFramebuffer;
    QVector4D mMouseWorldPosition;

    mSun = new Sun(this);

    mCamera = new Camera(this);
    mCamera->GetPosition()[2] = 20;
    mCamera->UpdateTransformation();
    mCamera->SetVerticalFov(80.0f);
    mCamera->SetZNear(0.1f);
    mCamera->SetZFar(10000.0f);

    mEarth = new Earth(this);
    mEarth->SetPosition(QVector3D(0, 0, 0));
    mEarth->SetScale(1, 1, 1);
    mEarth->Load("Resources/Models/Earth.obj", "Resources/Textures/world.topo.bathy.200406.3x5400x2700");

    return true;
}

void EarthRenderer::Renderer::Update(float ifps)
{
    if (mRotateEarth)
    {
        RotateEarth(mMouse.x, mMouse.y, mMouse.x + mMouse.dx, mMouse.y + mMouse.dy);
        mRotateEarth = false;
    }

    if (mRotateCamera)
    {
        RotateCamera(ifps);
        mRotateCamera = false;
    }
}

void EarthRenderer::Renderer::RotateEarth(int x, int y, int prevX, int prevY)
{
    QVector3D p0 = GetMouseWorldPosition(prevX, prevY);
    QVector3D p1 = GetMouseWorldPosition(x, y);
    QVector3D v0 = p0.normalized();
    QVector3D v1 = p1.normalized();

    const auto rotationAxis = QVector3D::crossProduct(p0, p1);
    const auto rotationAngle = Math::AngleBetween(p0, p1);
    const auto currentRotation = mEarth->GetRotation();
    const auto newRotation = QQuaternion::fromAxisAndAngle(rotationAxis, rotationAngle) * currentRotation;
    mEarth->SetRotation(newRotation);
}

void EarthRenderer::Renderer::RotateCamera(float ifps)
{
    auto tilt = mCamera->GetTilt();
    tilt -= mMouse.dw * ifps * 30;
    tilt = qBound(-89.0f, tilt, 89.0f);

    auto rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), tilt);

    const auto direction = mCamera->GetPosition().normalized();
    const auto distance = mCamera->GetPosition().length();
    auto newDirection = rotation * direction;

    mCamera->SetTilt(tilt);
    mCamera->SetRotation(rotation);
    mCamera->SetPosition(distance * direction);
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
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    RenderEarth();
    RenderForMousePosition();

    QOpenGLFramebufferObject::bindDefault();
    DrawGui();
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
    mEarthShader->SetUniformValue("cameraPosition", mCamera->GetPosition());
    mEarthShader->SetUniformValue("sun.direction", mSun->GetDirection());
    mEarthShader->SetUniformValue("sun.color", mSun->GetColor());
    mEarthShader->SetUniformValue("sun.ambient", mSun->GetAmbient());
    mEarthShader->SetUniformValue("sun.diffuse", mSun->GetDiffuse());
    mEarthShader->SetUniformValue("sun.specular", mSun->GetSpecular());
    mEarth->BindTexture();
    mEarth->Render();
    mEarth->ReleaseTexture();
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
    QtImGui::newFrame();
    glViewport(0, 0, mWidth, mHeight);
    ImGui::SetNextWindowSize(ImVec2(420, 420), ImGuiCond_FirstUseEver);
    ImGui::Begin("Debug");

    if (!ImGui::CollapsingHeader("Earth"))
    {
        ImGui::SliderFloat("Ambient##Earth", &mEarth->GetAmbient_NonConst(), 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Diffuse##Earth", &mEarth->GetDiffuse_NonConst(), 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Specular##Earth", &mEarth->GetSpecular_NonConst(), 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Shininess##Earth", &mEarth->GetShininess_NonConst(), 1.0f, 64.0f, "%.2f");

        if (ImGui::SliderFloat("Scale##Earth", &(mEarth->GetScale()[2]), 1.0f, 10.0f, "%.2f"))
        {
            mEarth->UpdateTransformation();
        }
    }

    if (!ImGui::CollapsingHeader("Sun"))
    {

        ImGui::SliderFloat("Ambient##Sun", &mSun->GetAmbient_NonConst(), 0.0f, 2.0f, "%.2f");
        ImGui::SliderFloat("Diffuse##Sun", &mSun->GetDiffuse_NonConst(), 0.0f, 2.0f, "%.2f");
        ImGui::SliderFloat("Specular##Sun", &mSun->GetSpecular_NonConst(), 0.0f, 2.0f, "%.2f");

        auto theta = mSun->GetTheta();
        auto phi = mSun->GetPhi();
        ImGui::SliderFloat("Theta", &theta, -180.0f, 180.0f, "%.1f");
        ImGui::SliderFloat("Phi", &phi, -90.0f, 90.0f, "%.1f");
        mSun->SetDirectionFromThetaPhi(theta, phi);
    }

    if (!ImGui::CollapsingHeader("Camera"))
    {
        if (ImGui::SliderFloat("Tilt##Camera", &mCamera->GetTilt_NonConst(), -89.0f, 89.0f, "%.2f"))
        {
            mRotateCamera = true;
        }

        if (ImGui::SliderFloat("Distance##Camera", &mCamera->GetPosition()[2], 10.0f, 100.0f, "%.2f"))
        {
            mCamera->UpdateTransformation();
            mRotateCamera = true;
        }
    }

    ImGui::Text("Shading");

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Latitude: %.6f, Longitude: %.6f)", mMouseWorldPosition[0], mMouseWorldPosition[1]);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    QtImGui::render();
}

void EarthRenderer::Renderer::Resize(int width, int height)
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

void EarthRenderer::Renderer::MousePressed(QMouseEvent* event)
{
    mMouse.button = event->button();

    if (mMouse.button == Qt::LeftButton)
    {
        mMouse.x = event->position().x();
        mMouse.y = event->position().y();
        mMouse.dx = 0;
        mMouse.dy = 0;
    }

    if (mMouse.button == Qt::RightButton)
    {
        mMouse.w = event->position().y();
        mMouse.dw = 0;
    }
}

void EarthRenderer::Renderer::MouseReleased(QMouseEvent* event)
{
    mMouse.button = Qt::NoButton;
}

void EarthRenderer::Renderer::MouseMoved(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        mRotateEarth = false;
        return;
    }

    if (mMouse.button == Qt::LeftButton)
    {
        mMouse.dx = mMouse.x - event->position().x();
        mMouse.dy = mMouse.y - event->position().y();

        mMouse.x = event->position().x();
        mMouse.y = event->position().y();

        mRotateEarth = true;
    }

    if (mMouse.button == Qt::RightButton)
    {

        mMouse.dw = mMouse.w - event->position().y();
        mMouse.w = event->position().y();

        mRotateCamera = true;
    }
}

void EarthRenderer::Renderer::WheelMoved(QWheelEvent* event)
{
    auto& z = mCamera->GetPosition()[2];

    if (event->angleDelta().y() < 0)
    {
        z = z / 0.99;
    }

    if (event->angleDelta().y() > 0)
    {
        z = z * 0.99;
    }

    z = qBound(10.0f, z, 1000.0f);

    mCamera->UpdateTransformation();
}

void EarthRenderer::Renderer::KeyPressed(QKeyEvent*)
{
}

void EarthRenderer::Renderer::KeyReleased(QKeyEvent*)
{
}