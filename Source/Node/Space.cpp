#include "Space.h"

#include "Core/Shader.h"
#include "Util/Logger.h"

GlobeRenderer::Space::Space(const Texture &SpaceTexture)
    : mTexture(SpaceTexture)
{}

void GlobeRenderer::Space::Render(Shader *pShader)
{
    glDisable(GL_DEPTH_TEST);
    pShader->SetSampler(mTexture.Name, mTexture.Unit, mTexture.Id, mTexture.Target);
    pShader->SetUniform("uBrightness", mBrightness);
    Cube::Render();
    glEnable(GL_DEPTH_TEST);
}
