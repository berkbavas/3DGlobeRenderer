#pragma once

#include "Node/Sphere.h"
#include "Structs/Texture.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QVector>
#include <map>

namespace GlobeRenderer
{
    class Globe : public Sphere
    {
      public:
        using Sphere::Sphere;

        Globe() = default;
        ~Globe() override = default;

        void Construct() override;
        void Render() override;
        void Destroy() override;

        void UpdateAllScale();

      private:
        DEFINE_MEMBER(float, AllScale, 1.0f);
        DEFINE_MEMBER(float, Ambient, 0.5f);
        DEFINE_MEMBER(float, Diffuse, 0.75f);
        DEFINE_MEMBER(float, Specular, 0.25f);
        DEFINE_MEMBER(float, Shininess, 4.0f);
        DEFINE_MEMBER(Texture, Texture);
    };
}
