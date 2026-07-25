#include "ArcballCamera.h"

#include "Core/Constants.h"
#include "Util/Math.h"

#include <cmath>

#include <QMatrix4x4>
#include <QVector3D>
#include <QtMath>

void GlobeRenderer::ArcballCamera::Update(float Ifps)
{
    // Theta
    if (!qFuzzyIsNull(mDeltaTheta))
    {
        const auto Angle = mDeltaTheta * mAngularSpeed * Ifps;
        mDeltaTheta -= Angle;
        RotateLocal(QVector3D(0, 1, 0), -Angle);
    }
    else
    {
        mDeltaTheta = 0.0f;
    }

    // Phi
    if (!qFuzzyIsNull(mDeltaPhi))
    {
        const auto Angle = mDeltaPhi * mAngularSpeed * Ifps;
        mDeltaPhi -= Angle;
        RotateLocal(QVector3D(1, 0, 0), -Angle);
    }
    else
    {
        mDeltaPhi = 0.0f;
    }

    // Roll
    if (!qFuzzyIsNull(mDeltaRoll))
    {
        const auto Angle = mDeltaRoll * mAngularSpeed * Ifps;
        mDeltaRoll -= Angle;
        RotateLocal(QVector3D(0, 0, 1), -Angle);
    }
    else
    {
        mDeltaRoll = 0.0f;
    }

    if (!qFuzzyIsNull(mDeltaVerticalFov))
    {
        mVerticalFov += mDeltaVerticalFov * 0.075;
        mDeltaVerticalFov = mDeltaVerticalFov * 0.925;
    }

    mVerticalFov = qBound(1.0f, mVerticalFov, 120.0f);

    SetPosition(-2.0 * GetViewDirection());
}

void GlobeRenderer::ArcballCamera::Reset()
{
    mDeltaPhi = 0.0f;
    mDeltaTheta = 0.0f;
    mDeltaRoll = 0.0f;
    mDeltaVerticalFov = 0.0f;
    mMouse.Reset();
}

bool GlobeRenderer::ArcballCamera::OnMousePressed(QMouseEvent* pEvent)
{
    bool Consumed = false;

    if (pEvent->buttons().testFlag(Qt::LeftButton))
    {
        mMouse.SetButtonPressed(Qt::LeftButton, true);
        mMouse.SetLastPressPosition(Qt::LeftButton, pEvent->position());
        Consumed = true;
    }

    if (pEvent->buttons().testFlag(Qt::MiddleButton))
    {
        mMouse.SetButtonPressed(Qt::MiddleButton, true);
        mMouse.SetLastPressPosition(Qt::MiddleButton, pEvent->position());
        Consumed = true;
    }

    return Consumed;
}

bool GlobeRenderer::ArcballCamera::OnMouseReleased(QMouseEvent* pEvent)
{
    bool Consumed = false;

    if (pEvent->button() == Qt::LeftButton)
    {
        mMouse.SetButtonPressed(Qt::LeftButton, false);
        Consumed = true;
    }

    if (pEvent->button() == Qt::MiddleButton)
    {
        mMouse.SetButtonPressed(Qt::MiddleButton, false);
        Consumed = true;
    }

    return Consumed;
}

bool GlobeRenderer::ArcballCamera::OnMouseMoved(QMouseEvent* pEvent)
{
    const auto Delta = pEvent->position() - mMouse.GetLastPressPosition(pEvent->button());
    const auto Movement = QVector2D(Delta.x() / mWidth, Delta.y() / mHeight);

    if (mMouse.IsButtonPressed(Qt::LeftButton))
    {
        mDeltaTheta += GetHorizontalFov() * Movement.x();
        mDeltaPhi += GetVerticalFov() * Movement.y();
    }

    if (mMouse.IsButtonPressed(Qt::MiddleButton))
    {
        mDeltaRoll += 90 * Movement.y();
    }

    mMouse.SetLastPressPosition(pEvent->button(), pEvent->position());

    return true;
}

bool GlobeRenderer::ArcballCamera::OnWheelMoved(QWheelEvent* pEvent)
{
    mDeltaVerticalFov += Math::Sign(pEvent->angleDelta().y()) * mVerticalFov * 0.075;
    return true;
}

bool GlobeRenderer::ArcballCamera::OnLeaveEvent(QEvent* pEvent)
{
    mMouse.Reset();
    return false;
}
