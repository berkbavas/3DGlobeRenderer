#include "Globe.h"

#include "Util/Logger.h"
#include "Util/Util.h"

void GlobeRenderer::Globe::Construct()
{
    Sphere::Construct();
}

void GlobeRenderer::Globe::Render()
{
    glActiveTexture(GL_TEXTURE0 + mTexture.unit);
    glBindTexture(mTexture.target, mTexture.id);

    Sphere::Render();

    glBindTexture(mTexture.target, 0);
}

void GlobeRenderer::Globe::Destroy()
{
    Sphere::Destroy();
}

void GlobeRenderer::Globe::UpdateAllScale()
{
    SetScale(mAllScale, mAllScale, mAllScale);
    UpdateTransformation();
}
