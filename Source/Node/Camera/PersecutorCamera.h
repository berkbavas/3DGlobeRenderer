#pragma once

#include "Node/Camera/PerspectiveCamera.h"
#include "Structs/Mouse.h"

namespace GlobeRenderer
{
    class PersecutorCamera : public PerspectiveCamera
    {
      public:
        PersecutorCamera() = default;

        void Update(float ifps) override;
        void Reset() override;

        void OnMousePressed(QMouseEvent* event) override;
        void OnMouseReleased(QMouseEvent* event) override;
        void OnMouseMoved(QMouseEvent* event) override;
        void OnWheelMoved(QWheelEvent* event) override;

        void SetTarget(Node* node);

      private:
        Node* mTarget{ nullptr };

        Mouse mMouse;

        DEFINE_MEMBER(float, Distance, 5.0f);
        DEFINE_MEMBER(float, Yaw, 0.0f);
        DEFINE_MEMBER(float, Pitch, 0.0f);
        DEFINE_MEMBER(float, WheelStep, 0.5f);
        DEFINE_MEMBER(float, AngularSpeed, 25.0f);
        DEFINE_MEMBER(float, AngularSpeedMultiplier, 1.0f);
        DEFINE_MEMBER(Qt::MouseButton, ActionReceiveButton, Qt::LeftButton)
        DEFINE_MEMBER(float, MinimumDistance, 0.1f);
        DEFINE_MEMBER(float, MaximumDistance, 100.0f);
    };

    using PersecutorCameraPtr = QSharedPointer<PersecutorCamera>;
}