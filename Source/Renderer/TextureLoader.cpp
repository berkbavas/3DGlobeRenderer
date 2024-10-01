#include "TextureLoader.h"

#include "Core/FailureBehaviour.h"
#include "Util/Logger.h"

#include <QImage>
#include <QOpenGLTexture>

GlobeRenderer::TextureLoader::TextureLoader()
{
    initializeOpenGLFunctions();
}

GLuint GlobeRenderer::TextureLoader::LoadTexture2D(const QString& path)
{
    LOG_DEBUG("TextureLoader::LoadTexture2D: Loading texture at {}", path.toStdString());

    QImage image = QImage(path);

    if (image.isNull())
    {
        LOG_FATAL("TextureLoader::LoadTexture2D: Image is null.");
        FailureBehaviour::Failure(FailureType::COULD_NOT_LOAD_TEXTURE);
    }

    QOpenGLTexture* texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setData(image);
    texture->setWrapMode(QOpenGLTexture::WrapMode::Repeat);
    texture->setMinMagFilters(QOpenGLTexture::Filter::LinearMipMapLinear, QOpenGLTexture::Filter::Linear);

    LOG_DEBUG("TextureLoader::LoadTexture2D: Texture has been loaded.");

    return texture->textureId();
}

GLuint GlobeRenderer::TextureLoader::LoadTextureCubeMap(const QString& folder, const QString& extension)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    QStringList paths;
    paths << folder + "/" + "px" + extension;
    paths << folder + "/" + "nx" + extension;
    paths << folder + "/" + "py" + extension;
    paths << folder + "/" + "ny" + extension;
    paths << folder + "/" + "pz" + extension;
    paths << folder + "/" + "nz" + extension;

    QVector<int> targets;

    targets << GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    targets << GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    targets << GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    targets << GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
    targets << GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    targets << GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

    for (unsigned int i = 0; i < 6; i++)
    {
        LOG_DEBUG("TextureLoader::LoadTextureCubeMap: Loading texture from '{}'", paths[i].toStdString());

        QImage image = QImage(paths[i]).mirrored().convertToFormat(QImage::Format_RGBA8888);

        if (image.isNull())
        {
            LOG_FATAL("TextureLoader::LoadTextureCubeMap: Image is null.");
            FailureBehaviour::Failure(FailureType::COULD_NOT_LOAD_TEXTURE);
        }

        glTexImage2D(targets[i], 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureId;
}