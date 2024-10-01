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

    if (!qFuzzyIsNull(mDeltaDistance))
    {
        const auto delta = mDeltaDistance * mLinearSpeed * ifps;
        mDeltaDistance -= delta;
        mDistance += delta;
    }
    else
    {
        mDeltaDistance = 0.0f;
    }

    mDistance = qBound(mMinimumDistance, mDistance, mMaximumDistance);

    SetPosition(-mDistance * GetViewDirection());
}

void GlobeRenderer::ArcballCamera::OnMousePressed(QMouseEvent* event)
{
    mMouse.x = event->pos().x() * mDevicePixelRatio;
    mMouse.y = event->pos().y() * mDevicePixelRatio;
    mMouse.button = event->button();
}

void GlobeRenderer::ArcballCamera::OnMouseReleased(QMouseEvent* event)
{
    mMouse.button = Qt::NoButton;
}

void GlobeRenderer::ArcballCamera::OnMouseMoved(QMouseEvent* event)
{
    const auto x = event->pos().x() * mDevicePixelRatio;
    const auto y = event->pos().y() * mDevicePixelRatio;

    const auto ndx = (x - mMouse.x) / mWidth;
    const auto ndy = (y - mMouse.y) / mHeight;

    const auto multiplier = mDistance / mMaximumDistance;

    if (mMouse.button == Qt::LeftButton)
    {
        mDeltaTheta += 360 * multiplier * ndx;
        mDeltaPhi += 180 * multiplier * ndy;
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
    mDeltaDistance += GetAdaptiveMultiplier() * Math::Sign(event->angleDelta().y());
}

float GlobeRenderer::ArcballCamera::GetAdaptiveMultiplier() const
{
    return 0.01f + (mDistance - mMinimumDistance) / (mMaximumDistance - mMinimumDistance);
}
