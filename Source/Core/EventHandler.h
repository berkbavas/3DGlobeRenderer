#pragma once

#include "Node/Camera.h"
#include "Node/Earth.h"

#include <QMouseEvent>
#include <QObject>

namespace EarthRenderer
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
        void UpdateEarthTransformation(float ifps);

        void MousePressed(QMouseEvent*);
        void MouseReleased(QMouseEvent*);
        void MouseMoved(QMouseEvent*);
        void WheelMoved(QWheelEvent*);

        void SetRenderer(Renderer* renderer);
        void Initialize();

      private:
        Renderer* mRenderer;
        Camera* mCamera;
        Earth* mEarth;

        float mCameraTiltSpeed{20.0f};
        float mCameraDistanceSpeed{10.0f};
        float mEarthRotationSpeed{30.0f};

        QVector3D mMouseEarthPosition;
        QVector3D mRotationAxis;
        float mEarthRotationAngleToBeConsumed{0};
        float mTiltAngleToBeConsumed{0};
        float mDistanceToBeConsumed{0};

        Mouse mMouse;

        Qt::MouseButton mPressedButton = Qt::NoButton;

        DEFINE_MEMBER(float, DevicePixelRatio, 1.0f);
    };
}