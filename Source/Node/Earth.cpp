#include "Earth.h"

#include "Util/Logger.h"
#include "Util/Util.h"

EarthRenderer::Earth::Earth(QObject* parent)
    : Node(parent)

{
}

void EarthRenderer::Earth::LoadModelData(const QString& path)
{
    initializeOpenGLFunctions();

    mModelData = new ModelData(path);
}

void EarthRenderer::Earth::Render()
{
    mModelData->Render();
}

void EarthRenderer::Earth::BindTextures()
{
    for (const auto& [unit, texture] : mTextures)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture->textureId());
    }
}
void EarthRenderer::Earth::ReleaseTextures()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void EarthRenderer::Earth::AddTexture(GLuint unit, const QString& path)
{
    if (const auto texture = Util::LoadTexture(path))
    {
        mTextures.emplace(unit, texture);
    }
    else
    {
        LOG_FATAL("Earth::AddTexture: Texture could not be loaded. Exiting...");
        std::exit(EXIT_FAILURE);
    }
}

void EarthRenderer::Earth::Rotate(const QVector3D& axis, float angle)
{
    SetRotation(QQuaternion::fromAxisAndAngle(axis, angle) * GetRotation());
}
