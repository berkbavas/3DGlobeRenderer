#include "ArcballCamera.h"

#include "Core/Constants.h"
#include "Util/Math.h"

#include <QMatrix4x4>
#include <QVector3D>
#include <QtMath>
#include <cmath>

void GlobeRenderer::ArcballCamera::Update(float ifps)
{
    // Theta
    if (!qFuzzyIsNull(mDeltaTheta))
    {
        const auto angle = mDeltaTheta * mAngularSpeed * ifps;
        mDeltaTheta -= angle;
        RotateLocal(QVector3D(0, 1, 0), -angle);
    }
    else
    {
        mDeltaTheta = 0.0f;
    }

    // Phi
    if (!qFuzzyIsNull(mDeltaPhi))
    {
        const auto angle = mDeltaPhi * mAngularSpeed * ifps;
        mDeltaPhi -= angle;
        RotateLocal(QVector3D(1, 0, 0), -angle);
    }
    else
    {
        mDeltaPhi = 0.0f;
    }

    // Roll
    if (!qFuzzyIsNull(mDeltaRoll))
    {
        const auto angle = mDeltaRoll * mAngularSpeed * ifps;
        mDeltaRoll -= angle;
        RotateLocal(QVector3D(0, 0, 1), -angle);
    }
    else
    {
        mDeltaRoll = 0.0f;
    }

    if (!qFuzzyIsNull(mDeltaVerticalFov))
    {
        const auto delta = mDeltaVerticalFov * mZoomSpeed * ifps;
        mDeltaVerticalFov -= delta;
        mVerticalFov += delta;
    }
    else
    {
        mDeltaVerticalFov = 0.0f;
    }

    SetPosition(-2.0 * GetViewDirection());
}

void GlobeRenderer::ArcballCamera::OnMousePressed(QMouseEvent* event)
{
    mMouse.x = event->position().x() * mDevicePixelRatio;
    mMouse.y = event->position().y() * mDevicePixelRatio;
    mMouse.button = event->button();
}

void GlobeRenderer::ArcballCamera::OnMouseReleased(QMouseEvent* event)
{
    mMouse.button = Qt::NoButton;
}

void GlobeRenderer::ArcballCamera::OnMouseMoved(QMouseEvent* event)
{
    const auto x = event->position().x() * mDevicePixelRatio;
    const auto y = event->position().y() * mDevicePixelRatio;

    const auto ndx = (x - mMouse.x) / mWidth;
    const auto ndy = (y - mMouse.y) / mHeight;

    if (mMouse.button == Qt::LeftButton)
    {
        mDeltaTheta += GetHorizontalFov() * ndx;
        mDeltaPhi += mVerticalFov * ndy;
    }
    else if (mMouse.button == Qt::MiddleButton)
    {
        mDeltaRoll += 90 * ndy;
    }

    mMouse.x = x;
    mMouse.y = y;
}

void GlobeRenderer::ArcballCamera::OnWheelMoved(QWheelEvent* event)
{
    mDeltaVerticalFov += 2 * std::tan(qDegreesToRadians(mVerticalFov)) * Math::Sign(event->angleDelta().y());
}