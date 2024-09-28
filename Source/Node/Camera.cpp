#include "Camera.h"

EarthRenderer::Camera::Camera(QObject* parent)
    : Node(parent)
{
}

QMatrix4x4 EarthRenderer::Camera::GetViewProjectionMatrix()
{
    return GetProjectionMatrix() * GetViewMatrix();
}

QMatrix4x4 EarthRenderer::Camera::GetViewMatrix()
{
    QMatrix4x4 viewMatrix;
    viewMatrix.rotate(GetRotation().conjugated());
    viewMatrix.translate(-GetPosition());

    return viewMatrix;
}

QMatrix4x4 EarthRenderer::Camera::GetRotationMatrix()
{
    auto rotation = GetViewMatrix();
    rotation.setColumn(3, QVector4D(0, 0, 0, 1));
    return rotation;
}

QVector3D EarthRenderer::Camera::GetViewDirection()
{
    return GetRotation() * QVector3D(0, 0, -1);
}

void EarthRenderer::Camera::Resize(int width, int height)
{
    mWidth = width;
    mHeight = height;
}

QMatrix4x4 EarthRenderer::Camera::GetProjectionMatrix()
{
    QMatrix4x4 projection;
    projection.perspective(mVerticalFov, float(mWidth) / float(mHeight), mZNear, mZFar);
    return projection;
}

void EarthRenderer::Camera::Reset()
{
    mZoomLevel = 100;
    mTilt = 0.0f;
}

void EarthRenderer::Camera::AddDistance(float delta)
{
    float& z = GetPosition()[2];
    z += delta;
    z = qBound(12.5f, z, 50.0f);

    UpdateTransformation();
}

float& EarthRenderer::Camera::GetDistance()
{
    return GetPosition()[2];
}

void EarthRenderer::Camera::AddTilt(float delta)
{
    mTilt += delta;
    mTilt = qBound(-89.0f, mTilt, 89.0f);

    UpdateTransformation();
}

void EarthRenderer::Camera::UpdateTransformation()
{
    auto rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), mTilt);

    const auto direction = GetPosition().normalized();
    const auto distance = GetPosition().length();
    auto newDirection = rotation * direction;

    SetRotation(rotation);
    SetPosition(distance * direction);
}
