#include "PersecutorCamera.h"

#include "Core/Constants.h"
#include "Util/Logger.h"
#include "Util/Math.h"

bool GlobeRenderer::PersecutorCamera::OnMousePressed(QMouseEvent* pEvent)
{
    if (ShouldIgnoreEvents())
    {
        return false;
    }

    bool Consumed = false;

    if (pEvent->buttons().testFlag(Qt::MiddleButton))
    {
        mMouse.SetButtonPressed(Qt::MiddleButton, true);
        mMouse.SetLastPressPosition(Qt::MiddleButton, pEvent->position());
        Consumed = true;
    }

    if (pEvent->buttons().testFlag(Qt::RightButton))
    {
        mMouse.SetButtonPressed(Qt::RightButton, true);
        mMouse.SetLastPressPosition(Qt::RightButton, pEvent->position());
        Consumed = true;
    }

    return Consumed;
}

bool GlobeRenderer::PersecutorCamera::OnMouseReleased(QMouseEvent* pEvent)
{
    if (ShouldIgnoreEvents())
    {
        return false;
    }

    mMouse.SetButtonPressed(pEvent->button(), false);

    return false;
}

bool GlobeRenderer::PersecutorCamera::OnMouseMoved(QMouseEvent* pEvent)
{
    if (ShouldIgnoreEvents())
    {
        return false;
    }

    bool Consumed = false;

    if (mMouse.IsButtonPressed(Qt::MiddleButton))
    {
        const auto Movement = pEvent->position() - mMouse.GetLastPressPosition(Qt::MiddleButton);

        mMouse.AddCumulativeMovement(Qt::MiddleButton, Movement);
        mMouse.SetLastPressPosition(Qt::MiddleButton, pEvent->position());

        Consumed = true;
    }

    if (mMouse.IsButtonPressed(Qt::RightButton))
    {
        const auto Movement = pEvent->position() - mMouse.GetLastPressPosition(Qt::RightButton);

        mMouse.AddCumulativeMovement(Qt::RightButton, Movement);
        mMouse.SetLastPressPosition(Qt::RightButton, pEvent->position());

        Consumed = true;
    }

    return Consumed;
}

bool GlobeRenderer::PersecutorCamera::OnWheelMoved(QWheelEvent* pEvent)
{
    if (ShouldIgnoreEvents())
    {
        return false;
    }

    if (pEvent->angleDelta().y() < 0)
    {
        mDistanceBuffer += mZoomStep;
    }

    if (pEvent->angleDelta().y() > 0)
    {
        mDistanceBuffer -= mZoomStep;
    }

    return true;
}

bool GlobeRenderer::PersecutorCamera::OnLeaveEvent(QEvent*)
{
    mMouse.Reset();
    return false;
}

void GlobeRenderer::PersecutorCamera::Update(float Ifps)
{
    HandleRotation(Ifps);
    HandleZoom(Ifps);
    HandleTranslation(Ifps);

    QVector3D TargetPosition(0, 0, 0);

    if (mTarget)
    {
        TargetPosition = mTarget->GetPosition();
    }

    auto NewRotation = QQuaternion::fromAxisAndAngle(POSITIVE_Y, mYaw) * QQuaternion::fromAxisAndAngle(POSITIVE_X, mPitch);
    auto NewWorldPosition = TargetPosition + mTranslation + mDistance * NewRotation * POSITIVE_Z;

    SetPosition(NewWorldPosition);
    SetRotation(NewRotation);
}

void GlobeRenderer::PersecutorCamera::HandleZoom(float Ifps)
{
    if (std::abs(mDistanceBuffer) < 0.1f)
    {
        mDistanceBuffer = 0;
    }

    float Step = mDistanceBuffer * mZoomSmoothness * Ifps;
    mDistance += Step;
    mDistanceBuffer -= Step;
    mDistance = qBound(MIN_DISTANCE, mDistance, MAX_DISTANCE);
}

void GlobeRenderer::PersecutorCamera::HandleRotation(float Ifps)
{
    const auto Step = mAngularSpeedSmoothness * mMouse.GetCumulativeMovement(Qt::MiddleButton);

    const auto ShouldRotate = Step != QPointF(0, 0);

    if (ShouldRotate)
    {
        const auto YawStep = mAngularSpeed * Step.x() * Ifps;
        const auto PitchStep = mAngularSpeed * Step.y() * Ifps;

        mYaw -= YawStep;
        mPitch -= PitchStep;
    }

    ClampAngles();

    // Consume deltas
    mMouse.ConsumeCumulativeMovement(Qt::MiddleButton, Step);
    mMouse.ResetCumulativeMovementIfAlmostZero(Qt::MiddleButton);
}

void GlobeRenderer::PersecutorCamera::HandleTranslation(float Ifps)
{
    const auto Step = mLinearSpeedSmoothness * mMouse.GetCumulativeMovement(Qt::RightButton);

    const auto ShouldTranslate = Step != QPointF(0, 0);

    if (ShouldTranslate)
    {
        const auto TranslationStepX = mLinearSpeed * Step.x() * Ifps;
        const auto TranslationStepY = mLinearSpeed * Step.y() * Ifps;

        const auto& Rotation = GetRotation();

        mTranslation -= TranslationStepX * (Rotation * POSITIVE_X); // (...) are important
        mTranslation -= TranslationStepY * (Rotation * NEGATIVE_Y);
    }

    // Consume deltas
    mMouse.ConsumeCumulativeMovement(Qt::RightButton, Step);
    mMouse.ResetCumulativeMovementIfAlmostZero(Qt::RightButton);
}

void GlobeRenderer::PersecutorCamera::Reset()
{
    mDistance = 2.5f;
    mYaw = 0.0f;
    mPitch = 0.0f;
    mTranslation = QVector3D(0, 0, 0);
    mMouse.Reset();
}

GlobeRenderer::Node* GlobeRenderer::PersecutorCamera::GetTarget() const
{
    return mTarget;
}

void GlobeRenderer::PersecutorCamera::SetTarget(Node* pNewTarget)
{
    LOG_DEBUG("PersecutorCamera::SetTarget: mTarget: {}, pNewTarget: {}", static_cast<void*>(mTarget), static_cast<void*>(pNewTarget));

    if (pNewTarget == nullptr)
    {
        LOG_INFO("PersecutorCamera::SetTarget: pNewTarget is nullptr.");
    }

    mTarget = pNewTarget;
    Reset();
}

void GlobeRenderer::PersecutorCamera::ClampAngles()
{
    Math::AddIfLess(mYaw, 0.0f, 360.0f);
    Math::AddIfGreater(mYaw, 360.0f, -360.0f);
    Math::AddIfLess(mPitch, 0.0f, 360.0f);
    Math::AddIfGreater(mPitch, 360.0f, -360.0f);
}

bool GlobeRenderer::PersecutorCamera::ShouldIgnoreEvents() const
{
    return false;
}
