#pragma once

#include "Node/Node.h"
#include "Util/Macros.h"

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QTimer>

namespace GlobeRenderer
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

        void Resize(int width, int height);
        void AddDistance(float delta);
        void AddTilt(float delta);
        void UpdateTransformation() override;

        float GetAspectRatio() const;
        float& GetDistance();
        float GetHorizontalFov() const;

        DEFINE_MEMBER(int, Width, 1600);
        DEFINE_MEMBER(int, Height, 900);
        DEFINE_MEMBER(float, ZNear, 0.01f);
        DEFINE_MEMBER(float, ZFar, 10.0f);
        DEFINE_MEMBER(float, VerticalFov, 60.0f);

        DEFINE_MEMBER(float, Tilt, 0.0f);
    };
}
