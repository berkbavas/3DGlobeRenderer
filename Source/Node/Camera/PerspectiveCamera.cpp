#include "PerspectiveCamera.h"

const QMatrix4x4& GlobeRenderer::PerspectiveCamera::GetProjectionMatrix()
{
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(mVerticalFov, GetAspectRatio(), GetZNear(), GetZFar());
    return mProjectionMatrix;
}

float GlobeRenderer::PerspectiveCamera::GetHorizontalFov() const
{
    const auto hfov = std::atan(std::tan(qDegreesToRadians(0.5 * mVerticalFov)) * GetAspectRatio()) * 2.0f;

    return qRadiansToDegrees(hfov);
}
