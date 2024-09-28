#pragma once

#include "Node/Node.h"
#include "Util/Macros.h"

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QTimer>

namespace EarthRenderer
{
    class Camera : public Node
    {
      public:
        explicit Camera(QObject* parent = nullptr);
        ~Camera() = default;

        QVector3D GetViewDirection();
        QMatrix4x4 GetProjectionMatrix();
        QMatrix4x4 GetViewProjectionMatrix();
        QMatrix4x4 GetViewMatrix();
        QMatrix4x4 GetRotationMatrix();

        void Reset();
        void Resize(int width, int height);

        void AddDistance(float delta);
        float& GetDistance();

        void AddTilt(float delta);

        void UpdateTransformation() override;

        DEFINE_MEMBER(int, Width, 1600);
        DEFINE_MEMBER(int, Height, 900);
        DEFINE_MEMBER(float, ZNear, 0.1f);
        DEFINE_MEMBER(float, ZFar, 100000.0f);
        DEFINE_MEMBER(float, VerticalFov, 60.0f);

        DEFINE_MEMBER(int, ZoomLevel, 100);
        DEFINE_MEMBER(float, Tilt, 0.0f);
    };
}
