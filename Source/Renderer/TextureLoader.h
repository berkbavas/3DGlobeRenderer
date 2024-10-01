#pragma once

#include <QOpenGLExtraFunctions>

namespace GlobeRenderer
{
    class TextureLoader : protected QOpenGLExtraFunctions
    {
      public:
        TextureLoader();

        GLuint LoadTexture2D(const QString& path);
        GLuint LoadTextureCubeMap(const QString& folder, const QString& extension);
    };
}