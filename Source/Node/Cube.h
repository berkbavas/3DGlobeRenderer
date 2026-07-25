#pragma once

#include "Node/Node.h"

#include <memory>

#include <QOpenGLExtraFunctions>
#include <QVector3D>
#include <QVector>

namespace GlobeRenderer
{
    class Cube : public Node, protected QOpenGLExtraFunctions
    {
      public:
        Cube();
        ~Cube() override;

      protected:
        void Render();

      private:
        static QVector<QVector3D> CreateVertices();

        GLuint mVAO{ 0 };
        GLuint mVBO{ 0 };
        int mNumberOfVertices{ 36 };
    };

    using CubePtr = std::unique_ptr<Cube>;
}