#pragma once

#include "Node/Node.h"
#include "Node/Vertex.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QVector>
#include <map>

namespace EarthRenderer
{
    class Earth : public Node, protected QOpenGLExtraFunctions
    {
      public:
        explicit Earth(QObject* parent = nullptr);
        ~Earth() = default;

        void Render();
        void Initialize(const QString& path);
        void BindTextures();
        void ReleaseTextures();
        void AddTexture(GLuint unit, const QString& path);

      private:
        DEFINE_MEMBER(float, Ambient, 1.0f);
        DEFINE_MEMBER(float, Diffuse, 1.0f);
        DEFINE_MEMBER(float, Specular, 0.25f);
        DEFINE_MEMBER(float, Shininess, 4.0f);
        DEFINE_MEMBER(GLuint, VertexArray, 0);
        DEFINE_MEMBER(GLuint, VertexBuffer, 0);

        QVector<Vertex> mVertices;
        std::map<GLuint, QOpenGLTexture*> mTextures;
    };
}
