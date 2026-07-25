#include "Sun.h"

#include "Util/Math.h"

#include <cmath>

float GlobeRenderer::Sun::GetTheta() const
{
    const auto Z = mDirection.z();
    const auto X = mDirection.x();
    return qRadiansToDegrees(std::atan2(Z, X));
}

float GlobeRenderer::Sun::GetPhi() const
{
    const auto X = mDirection.x();
    const auto Y = mDirection.y();
    const auto Z = mDirection.z();

    return qRadiansToDegrees(std::atan2(Y, std::sqrt(Z * Z + X * X)));
}

void GlobeRenderer::Sun::SetDirectionFromThetaPhi(float Theta, float Phi)
{
    const auto R = mDirection.length();
    const auto X = R * std::cos(qDegreesToRadians(Phi)) * std::cos(qDegreesToRadians(Theta));
    const auto Y = R * std::sin(qDegreesToRadians(Phi));
    const auto Z = R * std::cos(qDegreesToRadians(Phi)) * std::sin(qDegreesToRadians(Theta));

    mDirection = QVector3D(X, Y, Z);
}
