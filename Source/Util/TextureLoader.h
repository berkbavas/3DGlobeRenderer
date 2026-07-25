#pragma once

#include <memory>

#include <QOpenGLExtraFunctions>

namespace GlobeRenderer
{
    class TextureLoader : protected QOpenGLExtraFunctions
    {
      public:
        TextureLoader();

        GLuint LoadTexture2D(const QString& Path);
        GLuint LoadTextureCubeMap(const QString& Folder, const QString& Extension);
    };

    using TextureLoaderPtr = std::unique_ptr<TextureLoader>;
}