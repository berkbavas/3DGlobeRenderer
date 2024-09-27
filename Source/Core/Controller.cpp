#include "Controller.h"

#include "Renderer/Renderer.h"
#include "Renderer/Window.h"
#include "Util/Logger.h"

EarthRenderer::Controller::Controller(QObject* parent)
    : QObject(parent)
{
    LOG_DEBUG("Application starting...");

    mWindow = new Window;
    mRenderer = new Renderer;

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
    mRenderer->Initialize();
}

void EarthRenderer::Controller::Update(float ifps)
{
    mRenderer->Update(ifps);
}

void EarthRenderer::Controller::Render(float ifps)
{
    mRenderer->Render(ifps);
}

void EarthRenderer::Controller::KeyPressed(QKeyEvent* event)
{
    mRenderer->KeyPressed(event);
}

void EarthRenderer::Controller::KeyReleased(QKeyEvent* event)
{
    mRenderer->KeyReleased(event);
}

void EarthRenderer::Controller::Resize(int w, int h)
{
    mWindow->makeCurrent();
    mRenderer->Resize(w, h);
    mWindow->doneCurrent();
}

void EarthRenderer::Controller::MousePressed(QMouseEvent* event)
{
    mRenderer->MousePressed(event);
}

void EarthRenderer::Controller::MouseReleased(QMouseEvent* event)
{
    mRenderer->MouseReleased(event);
}

void EarthRenderer::Controller::MouseMoved(QMouseEvent* event)
{
    mRenderer->MouseMoved(event);
}

void EarthRenderer::Controller::WheelMoved(QWheelEvent* event)
{
    mRenderer->WheelMoved(event);
}
