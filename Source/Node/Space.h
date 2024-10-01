#pragma once

#include "Node/Renderable.h"
#include "Structs/Texture.h"

namespace GlobeRenderer
{
    class Space : public Renderable
    {
      public:
        Space() = default;

        void Construct() override;
        void Render() override;
        void Destroy() override;

      private:
        GLuint mVertexArray{ 0 };
        GLuint mVertexBuffer{ 0 };

        DEFINE_MEMBER(float, Brightness, 0.5f);
        DEFINE_MEMBER(Texture, Texture);
    };
}