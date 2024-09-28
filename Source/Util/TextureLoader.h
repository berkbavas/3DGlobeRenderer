#pragma once

#include "Util/Logger.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>

namespace GlobeRenderer
{
    class TextureLoader : protected QOpenGLExtraFunctions
    {
      public:
        TextureLoader();

        QOpenGLTexture* LoadTexture2D(const QString& path);
        GLuint LoadTextureCubeMap(const QString& folder, const QString& extension);
    };
}