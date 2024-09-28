#include "Globe.h"

#include "Util/Logger.h"
#include "Util/Util.h"

GlobeRenderer::Globe::Globe(QObject* parent)
    : Node(parent)

{
}

void GlobeRenderer::Globe::LoadModelData(const QString& path)
{
    initializeOpenGLFunctions();

    mModelData = new ModelData(path);
}

void GlobeRenderer::Globe::Render()
{
    mModelData->Render();
}

void GlobeRenderer::Globe::BindTextures()
{
    for (const auto& [unit, texture] : mTextures)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture->textureId());
    }
}
void GlobeRenderer::Globe::ReleaseTextures()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GlobeRenderer::Globe::AddTexture(GLuint unit, const QString& path)
{
    if (const auto texture = Util::LoadTexture(path))
    {
        mTextures.emplace(unit, texture);
    }
    else
    {
        LOG_FATAL("Globe::AddTexture: Texture could not be loaded. Exiting...");
        std::exit(EXIT_FAILURE);
    }
}

void GlobeRenderer::Globe::Rotate(const QVector3D& axis, float angle)
{
    SetRotation(QQuaternion::fromAxisAndAngle(axis, angle) * GetRotation());
}
