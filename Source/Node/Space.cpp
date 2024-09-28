#include "Space.h"

#include "Util/Logger.h"
#include "Util/TextureLoader.h"

#include <QImage>
#include <QString>

GlobeRenderer::Space::Space(QObject* parent)
    : QObject(nullptr)
{
    initializeOpenGLFunctions();
}

void GlobeRenderer::Space::LoadModelData(const QString& path)
{
    mModelData = new ModelData(path);
}

void GlobeRenderer::Space::LoadTextures(const QString& folder, const QString& extension)
{
    TextureLoader loader;
    mTextureId = loader.LoadTextureCubeMap(folder, extension);
}

void GlobeRenderer::Space::Render()
{
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
    mModelData->Render();
    glEnable(GL_DEPTH_TEST);
}
