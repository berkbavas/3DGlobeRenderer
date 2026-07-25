#pragma once

#include "Util/Macros.h"

#include <QVector3D>
#include <QVector4D>
#include <QtMath>

namespace GlobeRenderer
{
    class Sun
    {
      public:
        Sun() = default;

        float GetTheta() const;
        float GetPhi() const;
        void SetDirectionFromThetaPhi(float Theta, float Phi);

        DEFINE_MEMBER(float, Ambient, 1.0f);
        DEFINE_MEMBER(float, Diffuse, 1.0f);
        DEFINE_MEMBER(float, Specular, 1.0f);
        DEFINE_MEMBER(QVector3D, Color, 1, 1, 1);
        DEFINE_MEMBER(QVector3D, Direction, 1, 0, 0);
    };

    using SunPtr = std::unique_ptr<Sun>;
}
