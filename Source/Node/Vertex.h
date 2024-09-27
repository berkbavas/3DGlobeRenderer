#pragma once

#include <QVector3D>

namespace EarthRenderer
{
    struct Vertex
    {
        QVector3D position;
        QVector3D normal;
        QVector2D textureCoords;
    };
}