#pragma once

#include "Node/Camera/PerspectiveCamera.h"
#include "Structs/Mouse.h"

namespace GlobeRenderer
{
    class ArcballCamera : public PerspectiveCamera
    {
      public:
        ArcballCamera() = default;

        void Update(float ifps) override;

        void OnMousePressed(QMouseEvent* event) override;
        void OnMouseReleased(QMouseEvent* event) override;
        void OnMouseMoved(QMouseEvent* event) override;
        void OnWheelMoved(QWheelEvent* event) override;

        float GetAdaptiveMultiplier() const;

      private:
        Mouse mMouse;

        float mDeltaPhi{ 0 };
        float mDeltaTheta{ 0 };
        float mDeltaRoll{ 0 };

        float mDeltaDistance{ 0 };

        DEFINE_MEMBER(float, Distance, 5.0f);
        DEFINE_MEMBER(float, WheelStep, 0.5f);
        DEFINE_MEMBER(float, AngularSpeed, 10.0f);
        DEFINE_MEMBER(float, LinearSpeed, 8.0f);
        DEFINE_MEMBER(Qt::MouseButton, ActionReceiveButton, Qt::LeftButton)
        DEFINE_MEMBER(float, MinimumDistance, 1.25f);
        DEFINE_MEMBER(float, MaximumDistance, 10.0f);
        DEFINE_MEMBER(float, DevicePixelRatio, 1.0f)
    };

    using ArcballCameraPtr = QSharedPointer<ArcballCamera>;
}