#pragma once

#include <QVector3D>

namespace GlobeRenderer
{
    // Math Constants
    inline constexpr float EPSILON{ 1e-6f };

    // Vector Constants
    inline constexpr QVector3D ZERO_VECTOR_3D(0, 0, 0);
    inline constexpr QVector3D ORIGIN(0, 0, 0);
    inline constexpr QVector3D POSITIVE_Z(0, 0, 1);
    inline constexpr QVector3D NEGATIVE_Z(0, 0, -1);
    inline constexpr QVector3D POSITIVE_Y(0, 1, 0);
    inline constexpr QVector3D NEGATIVE_Y(0, -1, 0);
    inline constexpr QVector3D POSITIVE_X(1, 0, 0);
    inline constexpr QVector3D NEGATIVE_X(-1, 0, 0);
    inline constexpr QVector3D UP(0, 1, 0);
    inline constexpr QVector3D DOWN(0, -1, 0);
    inline constexpr QVector3D LEFT(-1, 0, 0);
    inline constexpr QVector3D RIGHT(1, 0, 0);
    inline constexpr QVector3D FORWARD(0, 0, -1);
    inline constexpr QVector3D BACKWARD(0, 0, 1);
}