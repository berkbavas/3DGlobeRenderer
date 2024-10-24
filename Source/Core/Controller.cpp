#include "Controller.h"

#include "Core/Window.h"
#include "Renderer/Renderer.h"
#include "Util/Logger.h"

#include <QtImGui.h>
#include <imgui.h>

GlobeRenderer::Controller::Controller(QObject* parent)
    : QObject(parent)
{
    mWindow = new Window;
    mRenderer = new Renderer;

    connect(mWindow, &GlobeRenderer::Window::Initialize, this, &GlobeRenderer::Controller::Initialize);
    connect(mWindow, &GlobeRenderer::Window::Render, this, &GlobeRenderer::Controller::Render);
    connect(mWindow, &GlobeRenderer::Window::Resize, this, &GlobeRenderer::Controller::Resize);
    connect(mWindow, &GlobeRenderer::Window::MousePressed, this, &GlobeRenderer::Controller::OnMousePressed);
    connect(mWindow, &GlobeRenderer::Window::MouseReleased, this, &GlobeRenderer::Controller::OnMouseReleased);
    connect(mWindow, &GlobeRenderer::Window::MouseMoved, this, &GlobeRenderer::Controller::OnMouseMoved);
    connect(mWindow, &GlobeRenderer::Window::WheelMoved, this, &GlobeRenderer::Controller::OnWheelMoved);
    connect(mWindow, &GlobeRenderer::Window::KeyPressed, this, &GlobeRenderer::Controller::OnKeyPressed);
    connect(mWindow, &GlobeRenderer::Window::KeyReleased, this, &GlobeRenderer::Controller::OnKeyReleased);
}

GlobeRenderer::Controller::~Controller()
{
    LOG_DEBUG("Controller::~Controller: Application closing...");
}

void GlobeRenderer::Controller::Run()
{
    LOG_DEBUG("Controller::Controller: Application starting...");

    mWindow->resize(mWidth, mHeight);
    mWindow->show();
}

void GlobeRenderer::Controller::Initialize()
{
    initializeOpenGLFunctions();

    QtImGui::initialize(mWindow);

    mRenderer->Initialize();
    mCamera = mRenderer->GetCamera();
    mGlobe = mRenderer->GetGlobe();
}

void GlobeRenderer::Controller::Render(float ifps)
{
    mDevicePixelRatio = mWindow->devicePixelRatio();
    mWidth = mWindow->width() * mDevicePixelRatio;
    mHeight = mWindow->height() * mDevicePixelRatio;

    mRenderer->SetDevicePixelRatio(mDevicePixelRatio);
    mRenderer->Render(ifps);

    QtImGui::newFrame();
    ImGui::Begin("Debug");

    mRenderer->DrawGui(ifps);

    ImGui::End();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, mWidth, mHeight);

    ImGui::Render();
    QtImGui::render();
}

void GlobeRenderer::Controller::OnKeyPressed(QKeyEvent* event)
{

    mCamera->OnKeyPressed(event);
}

void GlobeRenderer::Controller::OnKeyReleased(QKeyEvent* event)
{
    mCamera->OnKeyReleased(event);
}

void GlobeRenderer::Controller::Resize(int width, int height)
{
    mDevicePixelRatio = mWindow->devicePixelRatio();
    mWidth = width * mDevicePixelRatio;
    mHeight = height * mDevicePixelRatio;

    mWindow->makeCurrent();
    mRenderer->Resize(mWidth, mHeight);
    mWindow->doneCurrent();
}

void GlobeRenderer::Controller::OnMousePressed(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mCamera->OnMousePressed(event);
}

void GlobeRenderer::Controller::OnMouseReleased(QMouseEvent* event)
{
    mCamera->OnMouseReleased(event);
}

void GlobeRenderer::Controller::OnMouseMoved(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mCamera->OnMouseMoved(event);
    mRenderer->onMouseMoved(event);
}

void GlobeRenderer::Controller::OnWheelMoved(QWheelEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mCamera->OnWheelMoved(event);
}
