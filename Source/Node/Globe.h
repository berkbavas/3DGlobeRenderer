#pragma once

#include "Node/Sphere.h"
#include "Node/Texture.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QVector>

namespace GlobeRenderer
{
    class Shader;

    class Globe : public Sphere
    {
      public:
        explicit Globe(float Radius, int StackCount, int SectorCount, const Texture &GlobeTexture);

        void Render(Shader *pShader);

      private:
        Texture mTexture;

        DEFINE_MEMBER(float, Ambient, 0.5f);
        DEFINE_MEMBER(float, Diffuse, 0.75f);
        DEFINE_MEMBER(float, Specular, 0.25f);
        DEFINE_MEMBER(float, Shininess, 4.0f);
    };

    using GlobePtr = std::unique_ptr<Globe>;
}
