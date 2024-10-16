#pragma once

#include <QOpenGLExtraFunctions>

namespace GlobeRenderer
{
    class Quad : protected QOpenGLExtraFunctions
    {
      public:
        Quad();

        void Render();

      private:
        GLuint mVertexArray;
        GLuint mVertexBuffer;
    };
}
