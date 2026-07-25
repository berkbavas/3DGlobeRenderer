#pragma once

#include "Camera/PerspectiveCamera.h"
#include "Core/Mouse.h"

namespace GlobeRenderer
{
    class PersecutorCamera final : public PerspectiveCamera
    {
      public:
        PersecutorCamera() = default;

        bool OnMousePressed(QMouseEvent *) override;
        bool OnMouseReleased(QMouseEvent *) override;
        bool OnMouseMoved(QMouseEvent *) override;
        bool OnWheelMoved(QWheelEvent *) override;
        bool OnLeaveEvent(QEvent *) override;
        void Update(float Ifps) override;
        void Reset() override;

        Node *GetTarget() const;
        void SetTarget(Node *pNewTarget);

      private:
        void ClampAngles();
        bool ShouldIgnoreEvents() const;
        void HandleZoom(float ifps);
        void HandleRotation(float ifps);
        void HandleTranslation(float ifps);

        // Internal state
        Node *mTarget{ nullptr };
        Mouse mMouse;
        float mDistanceBuffer{ 0.0f };
        QVector3D mTranslation;

        float mYaw{ 0.0f };
        float mPitch{ 0.0f };
        float mDistance{ 2.5f };

        // Traits
        DEFINE_MEMBER(float, AngularSpeed, 10.0f);
        DEFINE_MEMBER(float, AngularSpeedSmoothness, 0.25f);
        DEFINE_MEMBER(float, LinearSpeed, 0.5f);
        DEFINE_MEMBER(float, LinearSpeedSmoothness, 0.25f);
        DEFINE_MEMBER(float, ZoomStep, 0.125f);
        DEFINE_MEMBER(float, ZoomSmoothness, 5.0f);

        static constexpr float MIN_DISTANCE{ 1.0f };
        static constexpr float MAX_DISTANCE{ 20.0f };
    };

    using PersecutorCameraPtr = std::unique_ptr<PersecutorCamera>;
}
