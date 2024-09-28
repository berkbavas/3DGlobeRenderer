#include "EventHandler.h"

#include "Renderer/Renderer.h"
#include "Util/Math.h"
#include "Util/Util.h"

#include <QMatrix4x4>
#include <QVector3D>
#include <QtMath>

GlobeRenderer::EventHandler::EventHandler(QObject* parent)
    : QObject(parent)
{
}

void GlobeRenderer::EventHandler::Update(float ifps)
{
    UpdateCameraTransformation(ifps);
    UpdateGlobeTransformation(ifps);
}

void GlobeRenderer::EventHandler::UpdateCameraTransformation(float ifps)
{
    if (!Util::IsBetween(-0.1f, mDistance, 0.1f))
    {
        const auto distance = mCameraDistanceSpeed * mDistance * ifps;
        mDistance -= distance;
        mCamera->AddDistance(distance);
    }
    else
    {
        mDistance = 0;
    }

    if (!Util::IsBetween(-0.1f, mTiltAngle, 0.1f))
    {
        const auto angle = mCameraTiltSpeed * mTiltAngle * ifps;
        mTiltAngle -= angle;
        mCamera->AddTilt(angle);
    }
    else
    {
        mTiltAngle = 0;
    }
}

void GlobeRenderer::EventHandler::UpdateGlobeTransformation(float ifps)
{
    if (!Util::IsBetween(-0.1f, mTheta, 0.1f))
    {
        const auto theta = mGlobeRotationSpeed * mTheta * ifps;
        mTheta -= theta;
        mGlobe->Rotate(mCamera->GetRotation() * QVector3D(1, 0, 0), theta);
    }
    else
    {
        mTheta = 0.0f;
    }

    if (!Util::IsBetween(-0.1f, mPhi, 0.1f))
    {
        const auto phi = mGlobeRotationSpeed * mPhi * ifps;
        mPhi -= phi;
        mGlobe->Rotate(mCamera->GetRotation() * QVector3D(0, 1, 0), phi);
    }
    else
    {
        mPhi = 0.0f;
    }

    if (!Util::IsBetween(-0.1f, mRoll, 0.1f))
    {
        const auto roll = mGlobeRotationSpeed * mRoll * ifps;
        mRoll -= roll;
        mGlobe->Rotate(mCamera->GetRotation() * QVector3D(0, 0, 1), roll);
    }
    else
    {
        mRoll = 0.0f;
    }
}

void GlobeRenderer::EventHandler::MousePressed(QMouseEvent* event)
{
    mMouse.x = event->pos().x() * mDevicePixelRatio;
    mMouse.y = event->pos().y() * mDevicePixelRatio;
    mPressedButton = event->button();
}

void GlobeRenderer::EventHandler::MouseReleased(QMouseEvent* event)
{
    mPressedButton = Qt::NoButton;
}

void GlobeRenderer::EventHandler::MouseMoved(QMouseEvent* event)
{
    const auto x = event->pos().x() * mDevicePixelRatio;
    const auto y = event->pos().y() * mDevicePixelRatio;

    if (mPressedButton == Qt::LeftButton)
    {
        mPhi += 10 * mCamera->GetDistance() * (x - mMouse.x) / mCamera->GetWidth();
        mTheta += 10 * mCamera->GetDistance() * (y - mMouse.y) / mCamera->GetHeight();
    }
    else if (mPressedButton == Qt::MiddleButton)
    {
        mRoll += 180 * (y - mMouse.y) / mCamera->GetHeight();
    }
    else if (mPressedButton == Qt::RightButton)
    {
        mTiltAngle += 0.05 * (mMouse.y - y);
    }

    mMouse.x = x;
    mMouse.y = y;
}

void GlobeRenderer::EventHandler::WheelMoved(QWheelEvent* event)
{
    mDistance += Util::Sign(event->angleDelta().y());
}

void GlobeRenderer::EventHandler::SetRenderer(Renderer* renderer)
{
    mRenderer = renderer;
}

void GlobeRenderer::EventHandler::Initialize()
{
    mCamera = mRenderer->GetCamera();
    mGlobe = mRenderer->GetGlobe();
}
