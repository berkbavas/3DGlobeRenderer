#include "Controller.h"

#include "Core/EventHandler.h"
#include "Renderer/Renderer.h"
#include "Renderer/Window.h"
#include "Util/Logger.h"

GlobeRenderer::Controller::Controller(QObject* parent)
    : QObject(parent)
{
    LOG_DEBUG("Application starting...");

    mWindow = new Window;
    mRenderer = new Renderer(this);
    mEventHandler = new EventHandler(this);
    mEventHandler->SetRenderer(mRenderer);

    connect(mWindow, &GlobeRenderer::Window::Initialize, this, &GlobeRenderer::Controller::Initialize);
    connect(mWindow, &GlobeRenderer::Window::Update, this, &GlobeRenderer::Controller::Update);
    connect(mWindow, &GlobeRenderer::Window::Render, this, &GlobeRenderer::Controller::Render);
    connect(mWindow, &GlobeRenderer::Window::Resize, this, &GlobeRenderer::Controller::Resize);
    connect(mWindow, &GlobeRenderer::Window::MousePressed, this, &GlobeRenderer::Controller::MousePressed);
    connect(mWindow, &GlobeRenderer::Window::MouseReleased, this, &GlobeRenderer::Controller::MouseReleased);
    connect(mWindow, &GlobeRenderer::Window::MouseMoved, this, &GlobeRenderer::Controller::MouseMoved);
    connect(mWindow, &GlobeRenderer::Window::WheelMoved, this, &GlobeRenderer::Controller::WheelMoved);
}

GlobeRenderer::Controller::~Controller()
{
    LOG_DEBUG("Application closing...");
}

void GlobeRenderer::Controller::Run()
{
    mWindow->resize(1600, 900);
    mWindow->show();
}

void GlobeRenderer::Controller::Initialize()
{
    initializeOpenGLFunctions();

    mRenderer->Initialize();
    mEventHandler->Initialize();
}

void GlobeRenderer::Controller::Update(float ifps)
{
    mDevicePixelRatio = mWindow->devicePixelRatio();
    mEventHandler->SetDevicePixelRatio(mDevicePixelRatio);
    mRenderer->SetDevicePixelRatio(mDevicePixelRatio);
    mEventHandler->Update(ifps);
}

void GlobeRenderer::Controller::Render(float ifps)
{
    mWidth = mWindow->width() * mDevicePixelRatio;
    mHeight = mWindow->height() * mDevicePixelRatio;

    mRenderer->Render(ifps);

    QOpenGLFramebufferObject::bindDefault();
    glViewport(0, 0, mWidth, mHeight);

    QtImGui::newFrame();

    ImGui::SetNextWindowSize(ImVec2(420, 420), ImGuiCond_FirstUseEver);

    ImGui::Begin("Debug");

    mRenderer->DrawGui();

    ImGui::End();
    ImGui::Render();
    QtImGui::render();
}

void GlobeRenderer::Controller::KeyPressed(QKeyEvent* event)
{
}

void GlobeRenderer::Controller::KeyReleased(QKeyEvent* event)
{
}

void GlobeRenderer::Controller::Resize(int width, int height)
{
    mWidth = width * mDevicePixelRatio;
    mHeight = height * mDevicePixelRatio;

    mWindow->makeCurrent();

    mRenderer->Resize(mWidth, mHeight);
    mWindow->doneCurrent();
}

void GlobeRenderer::Controller::MousePressed(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mEventHandler->MousePressed(event);
}

void GlobeRenderer::Controller::MouseReleased(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mEventHandler->MouseReleased(event);
}

void GlobeRenderer::Controller::MouseMoved(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mEventHandler->MouseMoved(event);
    mRenderer->MouseMoved(event);
}

void GlobeRenderer::Controller::WheelMoved(QWheelEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mEventHandler->WheelMoved(event);
}
