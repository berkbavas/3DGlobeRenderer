#include "PerspectiveCamera.h"

#include <QtMath>

PerspectiveCamera::PerspectiveCamera(QObject *parent)
    : Camera(parent)

{
    setVerticalFov(60.0f);
}

PerspectiveCamera::~PerspectiveCamera() {}

void PerspectiveCamera::setHorizontalFov(float newHorizontalFov)
{
    mHorizontalFov = newHorizontalFov;

    float halfHorizontalFovRadian = 0.5f * qDegreesToRadians(mHorizontalFov);
    float verticalFovRadian = 2 * atan(tan(halfHorizontalFovRadian) * (float(mWidth) / float(mHeight)));

    mVerticalFov = qRadiansToDegrees(verticalFovRadian);
}

void PerspectiveCamera::setVerticalFov(float newVerticalFov)
{
    mVerticalFov = newVerticalFov;

    float halfVerticalFovRadian = 0.5f * qDegreesToRadians(mVerticalFov);
    float horizontalFovRadian = 2 * atan(tan(halfVerticalFovRadian) / (float(mWidth) / float(mHeight)));

    mHorizontalFov = qRadiansToDegrees(horizontalFovRadian);
}

float PerspectiveCamera::verticalFov() const
{
    return mVerticalFov;
}

float PerspectiveCamera::horizontalFov() const
{
    return mHorizontalFov;
}

QMatrix4x4 PerspectiveCamera::projection()
{
    QMatrix4x4 projection;
    projection.perspective(mVerticalFov, float(mWidth) / float(mHeight), mZNear, mZFar);
    return projection;
}

void PerspectiveCamera::drawGUI()
{
    Camera::drawGUI();
}
