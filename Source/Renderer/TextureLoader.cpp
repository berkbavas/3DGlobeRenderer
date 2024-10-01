#include "TextureLoader.h"

#include "Core/FailureBehaviour.h"
#include "Util/Logger.h"

#include <QImage>

GlobeRenderer::TextureLoader::TextureLoader()
{
    initializeOpenGLFunctions();
}

GLuint GlobeRenderer::TextureLoader::LoadTexture2D(const QString& path)
{
    LOG_DEBUG("TextureLoader::LoadTexture2D: Loading texture at {}", path.toStdString());

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    QImage image = QImage(path);

    if (image.isNull())
    {
        LOG_FATAL("TextureLoader::LoadTexture2D: Image is null.");
        FailureBehaviour::Failure(FailureType::COULD_NOT_LOAD_TEXTURE);
    }

    image = image.convertToFormat(QImage::Format_RGBA8888);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    LOG_DEBUG("TextureLoader::LoadTexture2D: Texture has been loaded.");

    return textureId;
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

        QImage image = QImage(paths[i]);

        if (image.isNull())
        {
            LOG_FATAL("TextureLoader::LoadTextureCubeMap: Image is null.");
            FailureBehaviour::Failure(FailureType::COULD_NOT_LOAD_TEXTURE);
        }

        image = image.mirrored().convertToFormat(QImage::Format_RGBA8888);

        glTexImage2D(targets[i], 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureId;
}