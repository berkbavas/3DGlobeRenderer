#pragma once

#include "Camera/PerspectiveCamera.h"
#include "Core/Mouse.h"

namespace GlobeRenderer
{
    class PersecutorCamera final : public PerspectiveCamera
    {
      public:
        PersecutorCamera() = default;

        bool OnMousePressed(QMouseEvent* pEvent) override;
        bool OnMouseReleased(QMouseEvent* pEvent) override;
        bool OnMouseMoved(QMouseEvent* pEvent) override;
        bool OnWheelMoved(QWheelEvent* pEvent) override;
        bool OnLeaveEvent(QEvent* pEvent) override;
        void Update(float Ifps) override;
        void Reset() override;

        Node* GetTarget() const;
        void SetTarget(Node* pNewTarget);

      private:
        void ClampAngles();
        bool ShouldIgnoreEvents() const;
        void HandleZoom(float Ifps);
        void HandleRotation(float Ifps);
        void HandleTranslation(float Ifps);

        // Internal state
        Node* mTarget{ nullptr };
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
