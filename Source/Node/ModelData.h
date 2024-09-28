#pragma once

#include "Node/Vertex.h"
#include "Util/Macros.h"

#include <QOpenGLExtraFunctions>
#include <QString>
#include <QVector>
#include <map>

namespace GlobeRenderer
{
    class ModelData : protected QOpenGLExtraFunctions
    {
      public:
        ModelData(const QString& path);
        ~ModelData() = default;

        void Render();

        DEFINE_MEMBER(GLuint, VertexArray, 0);
        DEFINE_MEMBER(GLuint, VertexBuffer, 0);
        DEFINE_MEMBER(QVector<Vertex>, Vertices, QVector<Vertex>());
    };
}
