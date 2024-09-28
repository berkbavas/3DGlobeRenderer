#pragma once

#include "Node/Vertex.h"

#include <QByteArray>
#include <QOpenGLTexture>
#include <QString>

namespace GlobeRenderer
{
    class Util
    {
      public:
        Util() = delete;

        static QByteArray GetBytes(const QString& path);
        static QVector<Vertex> Parse(const QString& path);
        static QOpenGLTexture* LoadTexture(const QString& path);

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
