#include "Globe.h"

#include "Core/Shader.h"

GlobeRenderer::Globe::Globe(float Radius, int StackCount, int SectorCount, const Texture &GlobeTexture)
    : Sphere(Radius, StackCount, SectorCount)
    , mTexture(GlobeTexture)
{}

void GlobeRenderer::Globe::Render(Shader *pShader)
{
    pShader->SetSampler(mTexture.Name, mTexture.Unit, mTexture.Id, mTexture.Target);
    Sphere::Render();
}
