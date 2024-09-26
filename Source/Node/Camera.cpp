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