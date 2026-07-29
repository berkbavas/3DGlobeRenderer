#include "TextureLoader.h"

#include "Util/Logger.h"

#include <QImage>

GlobeRenderer::TextureLoader::TextureLoader()
{
    initializeOpenGLFunctions();
}

GLuint GlobeRenderer::TextureLoader::LoadTexture2D(const QString& Path)
{
    LOG_DEBUG("TextureLoader::LoadTexture2D: Loading texture at {}", Path.toStdString());

    const auto Image = QImage(Path);

    if (Image.isNull())
    {
        GR_EXIT_FAILURE("TextureLoader::LoadTexture2D: Image is null: {}", Path.toStdString());
    }

    const auto ConvertedImage = Image.convertToFormat(QImage::Format_RGBA8888);

    GLuint TextureId;
    glGenTextures(1, &TextureId);
    glBindTexture(GL_TEXTURE_2D, TextureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ConvertedImage.width(), ConvertedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ConvertedImage.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    LOG_DEBUG("TextureLoader::LoadTexture2D: Texture has been loaded.");

    return TextureId;
}

GLuint GlobeRenderer::TextureLoader::LoadTextureCubeMap(const QString& Folder, const QString& Extension)
{
    QStringList Paths;
    Paths << Folder + "/" + "px" + Extension;
    Paths << Folder + "/" + "nx" + Extension;
    Paths << Folder + "/" + "py" + Extension;
    Paths << Folder + "/" + "ny" + Extension;
    Paths << Folder + "/" + "pz" + Extension;
    Paths << Folder + "/" + "nz" + Extension;

    QVector<int> Targets;

    Targets << GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    Targets << GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    Targets << GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    Targets << GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
    Targets << GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    Targets << GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

    GLuint TextureId;
    glGenTextures(1, &TextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureId);

    for (int i = 0; i < 6; i++)
    {
        LOG_DEBUG("TextureLoader::LoadTextureCubeMap: Loading texture from '{}'", Paths[i].toStdString());

        const auto Image = QImage(Paths[i]);

        if (Image.isNull())
        {
            GR_EXIT_FAILURE("TextureLoader::LoadTextureCubeMap: Image is null: {}", Paths[i].toStdString());
        }

        const auto ConvertedImage = Image.mirrored().convertToFormat(QImage::Format_RGBA8888);

        glTexImage2D(Targets[i], 0, GL_RGBA, ConvertedImage.width(), ConvertedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ConvertedImage.bits());
    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return TextureId;
}
