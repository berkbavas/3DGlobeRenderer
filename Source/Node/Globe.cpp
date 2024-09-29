#include "Globe.h"

#include "Util/Logger.h"
#include "Util/Util.h"

GlobeRenderer::Globe::Globe(QObject* parent)
    : Node(parent)

{
    mTextureLoader = new TextureLoader;
    mSphere = new Sphere(1, 128, 128, true);
    mSphere->CreateOpenGLStuff();

    LOG_DEBUG("Globe::Globe: \n {}", mSphere->ToString());

    initializeOpenGLFunctions();
}

void GlobeRenderer::Globe::Render()
{
    mSphere->Render();
}

void GlobeRenderer::Globe::BindTextures()
{
    for (const auto& [unit, textureId] : mTextures)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(mTextureTargets[unit], textureId);
    }
}
void GlobeRenderer::Globe::ReleaseTextures()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GlobeRenderer::Globe::AddTexture(GLuint unit, const QString& path)
{
    if (const auto texture = mTextureLoader->LoadTexture2D(path))
    {
        mTextures.emplace(unit, texture->textureId());
        mTextureTargets.emplace(unit, GL_TEXTURE_2D);
    }
    else
    {
        LOG_FATAL("Globe::AddTexture: Texture could not be loaded. Exiting...");
        std::exit(EXIT_FAILURE);
    }
}

void GlobeRenderer::Globe::AddTextureCubeMap(GLuint unit, const QString& folder, const QString& extension)
{
    if (const auto textureId = mTextureLoader->LoadTextureCubeMap(folder, extension))
    {
        mTextures.emplace(unit, textureId);
        mTextureTargets.emplace(unit, GL_TEXTURE_CUBE_MAP);
    }
    else
    {
        LOG_FATAL("Globe::AddTextureCubeMap: Texture could not be loaded. Exiting...");
        std::exit(EXIT_FAILURE);
    }
}

void GlobeRenderer::Globe::Rotate(const QVector3D& axis, float angle)
{
    SetRotation(QQuaternion::fromAxisAndAngle(axis, angle) * GetRotation());
}
