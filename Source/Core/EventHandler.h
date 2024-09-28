#pragma once

#include "Node/Camera.h"
#include "Node/Globe.h"

#include <QMouseEvent>
#include <QObject>

namespace GlobeRenderer
{
    struct Mouse
    {
        float x{0};
        float y{0};
    };

    class Renderer;

    class EventHandler : public QObject
    {

      public:
        explicit EventHandler(QObject* parent = nullptr);

        void DrawGui();
        void Update(float ifps);

        void UpdateCameraTransformation(float ifps);
        void UpdateGlobeTransformation(float ifps);

        void MousePressed(QMouseEvent*);
        void MouseReleased(QMouseEvent*);
        void MouseMoved(QMouseEvent*);
        void WheelMoved(QWheelEvent*);

        void SetRenderer(Renderer* renderer);
        void Initialize();

      private:
        Renderer* mRenderer;
        Camera* mCamera;
        Globe* mGlobe;

        float mCameraTiltSpeed{20.0f};
        float mCameraDistanceSpeed{10.0f};
        float mGlobeRotationSpeed{10.0f};

        // Camera
        float mTiltAngle{0};
        float mDistance{0};

        // Globe
        float mPhi{0};
        float mTheta{0};
        float mRoll{0};

        Mouse mMouse;

        Qt::MouseButton mPressedButton = Qt::NoButton;

        DEFINE_MEMBER(float, DevicePixelRatio, 1.0f);
    };
}