#include "PerspectiveCamera.h"

const QMatrix4x4& GlobeRenderer::PerspectiveCamera::GetProjectionMatrix()
{
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(mVerticalFov, GetAspectRatio(), GetZNear(), GetZFar());
    return mProjectionMatrix;
}

float GlobeRenderer::PerspectiveCamera::GetHorizontalFov() const
{
    const auto hfov = qAtan(qTan(mVerticalFov / 2.0) / GetAspectRatio()) * 2.0f;

    return qAbs(qRadiansToDegrees(hfov));
}
