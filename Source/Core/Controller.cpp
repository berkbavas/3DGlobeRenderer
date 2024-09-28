#include "Controller.h"

#include "Core/EventHandler.h"
#include "Renderer/Renderer.h"
#include "Renderer/Window.h"
#include "Util/Logger.h"

EarthRenderer::Controller::Controller(QObject* parent)
    : QObject(parent)
{
    LOG_DEBUG("Application starting...");

    mWindow = new Window;
    mRenderer = new Renderer(this);
    mEventHandler = new EventHandler(this);
    mEventHandler->SetRenderer(mRenderer);

    connect(mWindow, &EarthRenderer::Window::Initialize, this, &EarthRenderer::Controller::Initialize);
    connect(mWindow, &EarthRenderer::Window::Update, this, &EarthRenderer::Controller::Update);
    connect(mWindow, &EarthRenderer::Window::Render, this, &EarthRenderer::Controller::Render);
    connect(mWindow, &EarthRenderer::Window::Resize, this, &EarthRenderer::Controller::Resize);
    connect(mWindow, &EarthRenderer::Window::MousePressed, this, &EarthRenderer::Controller::MousePressed);
    connect(mWindow, &EarthRenderer::Window::MouseReleased, this, &EarthRenderer::Controller::MouseReleased);
    connect(mWindow, &EarthRenderer::Window::MouseMoved, this, &EarthRenderer::Controller::MouseMoved);
    connect(mWindow, &EarthRenderer::Window::WheelMoved, this, &EarthRenderer::Controller::WheelMoved);
}

EarthRenderer::Controller::~Controller()
{
    LOG_DEBUG("Application closing...");
}

void EarthRenderer::Controller::Run()
{
    mWindow->resize(1600, 900);
    mWindow->show();
}

void EarthRenderer::Controller::Initialize()
{
    initializeOpenGLFunctions();

    mRenderer->Initialize();
    mEventHandler->Initialize();
}

void EarthRenderer::Controller::Update(float ifps)
{
    mDevicePixelRatio = mWindow->devicePixelRatio();
    mEventHandler->SetDevicePixelRatio(mDevicePixelRatio);
    mEventHandler->Update(ifps);
}

void EarthRenderer::Controller::Render(float ifps)
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
    mEventHandler->DrawGui();

    ImGui::End();
    ImGui::Render();
    QtImGui::render();
}

void EarthRenderer::Controller::KeyPressed(QKeyEvent* event)
{
}

void EarthRenderer::Controller::KeyReleased(QKeyEvent* event)
{
}

void EarthRenderer::Controller::Resize(int width, int height)
{
    mWidth = width * mDevicePixelRatio;
    mHeight = height * mDevicePixelRatio;

    mWindow->makeCurrent();

    mRenderer->Resize(mWidth, mHeight);
    mWindow->doneCurrent();
}

void EarthRenderer::Controller::MousePressed(QMouseEvent* event)
{
    mEventHandler->MousePressed(event);
}

void EarthRenderer::Controller::MouseReleased(QMouseEvent* event)
{
    mEventHandler->MouseReleased(event);
}

void EarthRenderer::Controller::MouseMoved(QMouseEvent* event)
{
    mEventHandler->MouseMoved(event);
}

void EarthRenderer::Controller::WheelMoved(QWheelEvent* event)
{
    mEventHandler->WheelMoved(event);
}
