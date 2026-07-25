#pragma once

#include "Camera/PerspectiveCamera.h"
#include "Core/Mouse.h"

namespace GlobeRenderer
{
    class ArcballCamera : public PerspectiveCamera
    {
      public:
        ArcballCamera() = default;

        void Update(float Ifps) override;
        void Reset() override;

        bool OnMousePressed(QMouseEvent* pEvent) override;
        bool OnMouseReleased(QMouseEvent* pEvent) override;
        bool OnMouseMoved(QMouseEvent* pEvent) override;
        bool OnWheelMoved(QWheelEvent* pEvent) override;
        bool OnLeaveEvent(QEvent* pEvent) override;

      private:
        Mouse mMouse;

        float mDeltaPhi{ 0 };
        float mDeltaTheta{ 0 };
        float mDeltaRoll{ 0 };
        float mDeltaVerticalFov{ 0 };

        DEFINE_MEMBER(float, WheelStep, 0.5f);
        DEFINE_MEMBER(float, AngularSpeed, 10.0f);
    };

    using ArcballCameraPtr = std::unique_ptr<ArcballCamera>;
}