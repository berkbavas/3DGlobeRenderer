#pragma once

#include "Node/Vertex.h"

#include <QByteArray>
#include <QOpenGLTexture>
#include <QString>


namespace EarthRenderer
{
    class Util
    {
      public:
        Util() = delete;

        static QByteArray GetBytes(const QString& path);
        static QVector<Vertex> Parse(const QString& path);
        static QOpenGLTexture* LoadTexture(const QString& path);
    };
}
