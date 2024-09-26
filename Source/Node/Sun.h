#pragma once

#include "Util/Macros.h"

#include <QObject>
#include <QVector3D>

namespace EarthRenderer
{
    class Sun : public QObject
    {
      public:
        explicit Sun(QObject* parent = nullptr)
            : QObject(parent)
        {
        }

        float GetTheta() const
        {
            const auto z = mDirection.z();
            const auto x = mDirection.x();
            return qRadiansToDegrees(atan2(z, x));
        }

        float GetPhi() const
        {
            const auto x = mDirection.x();
            const auto y = mDirection.y();
            const auto z = mDirection.z();

            return qRadiansToDegrees(atan2(y, sqrt(z * z + x * x)));
        }

        void SetDirectionFromThetaPhi(float theta, float phi)
        {
            const auto r = mDirection.length();
            const auto x = r * cos(qDegreesToRadians(phi)) * cos(qDegreesToRadians(theta));
            const auto y = r * sin(qDegreesToRadians(phi));
            const auto z = r * cos(qDegreesToRadians(phi)) * sin(qDegreesToRadians(theta));

            mDirection = QVector3D(x, y, z);
        }

        DEFINE_MEMBER(float, Ambient, 1.0f);
        DEFINE_MEMBER(float, Diffuse, 1.0f);
        DEFINE_MEMBER(float, Specular, 1.0f);
        DEFINE_MEMBER(QVector4D, Color, 1, 1, 1, 1);
        DEFINE_MEMBER(QVector3D, Direction, 1, 0, 0);
    };
}
