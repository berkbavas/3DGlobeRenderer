#pragma once

#include "Node/Cube.h"
#include "Node/Texture.h"
#include "Util/Macros.h"

#include <QOpenGLExtraFunctions>
#include <QVector3D>
#include <QVector>

namespace GlobeRenderer
{
    class Shader;

    class Space : public Cube
    {
      public:
        explicit Space(const Texture &SpaceTexture);

        void Render(Shader *pShader);

      private:
        Texture mTexture;

        DEFINE_MEMBER(float, Brightness, 0.5f);
    };

    using SpacePtr = std::unique_ptr<Space>;
}