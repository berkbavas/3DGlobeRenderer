#include "Space.h"

#include "Util/Logger.h"

#include <QImage>
#include <QString>

EarthRenderer::Space::Space(QObject* parent)
    : QObject(nullptr)
{
    initializeOpenGLFunctions();
}

void EarthRenderer::Space::LoadModelData(const QString& path)
{
    mModelData = new ModelData(path);
}

void EarthRenderer::Space::LoadTextures(const QString& folder, const QString& extension)
{
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);

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
        LOG_DEBUG("Space::LoadTextures: Loading texture from '{}'", paths[i].toStdString());

        QImage image = QImage(paths[i]).mirrored().convertToFormat(QImage::Format_RGBA8888);

        if (image.isNull())
        {
            LOG_FATAL("Space::LoadTextures: Image is null. Exiting...");
            std::exit(EXIT_FAILURE);
        }

        glTexImage2D(targets[i], 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void EarthRenderer::Space::Render()
{
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
    mModelData->Render();
    glEnable(GL_DEPTH_TEST);
}
