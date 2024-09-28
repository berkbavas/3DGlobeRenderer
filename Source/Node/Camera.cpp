#include "Camera.h"

#include <QtMath>
#include <cmath>

GlobeRenderer::Camera::Camera(QObject* parent)
    : Node(parent)
{
}

QMatrix4x4 GlobeRenderer::Camera::GetViewProjectionMatrix()
{
    return GetProjectionMatrix() * GetViewMatrix();
}

QMatrix4x4 GlobeRenderer::Camera::GetViewMatrix()
{
    QMatrix4x4 viewMatrix;
    viewMatrix.rotate(GetRotation().conjugated());
    viewMatrix.translate(-GetPosition());

    return viewMatrix;
}

QMatrix4x4 GlobeRenderer::Camera::GetRotationMatrix()
{
    auto rotation = GetViewMatrix();
    rotation.setColumn(3, QVector4D(0, 0, 0, 1));
    return rotation;
}

QVector3D GlobeRenderer::Camera::GetViewDirection()
{
    return GetRotation() * QVector3D(0, 0, -1);
}

void GlobeRenderer::Camera::Resize(int width, int height)
{
    mWidth = width;
    mHeight = height;
}

QMatrix4x4 GlobeRenderer::Camera::GetProjectionMatrix()
{
    QMatrix4x4 projection;
    projection.perspective(mVerticalFov, GetAspectRatio(), mZNear, mZFar);
    return projection;
}

void GlobeRenderer::Camera::Reset()
{
    mZoomLevel = 100;
    mTilt = 0.0f;
}

void GlobeRenderer::Camera::AddDistance(float delta)
{
    float& z = GetPosition()[2];
    z += delta;
    z = qBound(12.5f, z, 50.0f);

    UpdateTransformation();
}

float& GlobeRenderer::Camera::GetDistance()
{
    return GetPosition()[2];
}

void GlobeRenderer::Camera::AddTilt(float delta)
{
    mTilt += delta;
    mTilt = qBound(-89.0f, mTilt, 89.0f);

    UpdateTransformation();
}

void GlobeRenderer::Camera::UpdateTransformation()
{
    auto rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), mTilt);

    const auto direction = GetPosition().normalized();
    const auto distance = GetPosition().length();
    auto newDirection = rotation * direction;

    SetRotation(rotation);
    SetPosition(distance * direction);
}

float GlobeRenderer::Camera::GetAspectRatio() const
{
    return float(mWidth) / float(mHeight);
}
float GlobeRenderer::Camera::GetHorizontalFov() const
{
    const auto ar = GetAspectRatio();
    const auto vfov = mVerticalFov;
    const auto hfov = qAtan(qTan(vfov / 2.0) / ar) * 2.0f;

    return qAbs(qRadiansToDegrees(hfov));
}