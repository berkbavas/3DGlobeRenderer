#pragma once

#include <QMatrix4x4>
#include <QQuaternion>
#include <QtMath>

namespace GlobeRenderer
{
    class Math
    {
      private:
        Math() = delete;

      public:
        static QQuaternion RotateX(float angleRadians);
        static QQuaternion RotateY(float angleRadians);
        static QQuaternion RotateZ(float angleRadians);
        static QQuaternion Invert(const QQuaternion& rotation);
        static float AngleBetween(const QVector3D& v1, const QVector3D& v2, const QVector3D& left);
        static float AngleBetween(const QVector3D& v1, const QVector3D& v2);
        static void GetEulerDegrees(const QQuaternion& rotation, float& yaw, float& pitch, float& roll);
        static QQuaternion ConstructFromEulerDegrees(float yaw, float pitch, float roll);
        static QVector3D ConstructFromLatLon(float lat, float lon);

        template<typename T>
        static int Sign(T val)
        {
            return (T(0) < val) - (val < T(0));
        }

        template<typename T>
        static bool IsBetween(T min, T val, T max)
        {
            return min <= val && val <= max;
        }
    };
}
