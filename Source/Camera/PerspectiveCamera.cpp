#include "PerspectiveCamera.h"

QMatrix4x4 GlobeRenderer::PerspectiveCamera::GetProjectionMatrix() const
{
    QMatrix4x4 ProjectionMatrix;
    ProjectionMatrix.perspective(mVerticalFov, GetAspectRatio(), mZNear, mZFar);
    return ProjectionMatrix;
}

float GlobeRenderer::PerspectiveCamera::GetHorizontalFov() const
{
    const auto VerticalFovRadians = qDegreesToRadians(mVerticalFov);
    const auto AspectRatio = GetAspectRatio();
    const auto HorizontalFovRadians = 2 * std::atan(std::tan(VerticalFovRadians / 2) * AspectRatio);
    return qRadiansToDegrees(HorizontalFovRadians);
}

float GlobeRenderer::PerspectiveCamera::GetAspectRatio() const
{
    return static_cast<float>(mWidth) / static_cast<float>(mHeight);
}
