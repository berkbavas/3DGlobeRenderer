#pragma once

#include "Node/Node.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>

namespace EarthRenderer
{
    struct Vertex
    {
        QVector3D position;
        QVector3D normal;
        QVector2D textureCoords;
    };

    class Earth : public Node, protected QOpenGLExtraFunctions
    {
      public:
        explicit Earth(QObject* parent = nullptr);
        ~Earth() = default;

        void Load(const QString& pathToModel, const QString& pathToTexture);

        void BindTexture();
        void Render();
        void ReleaseTexture();

      private:
        void Parse(const QString& pathToModel);

        void LoadTexture(const QString& pathToTexture);
        void ConstructOpenGLStuff();

        DEFINE_MEMBER(float, Ambient, 1.0f);
        DEFINE_MEMBER(float, Diffuse, 1.0f);
        DEFINE_MEMBER(float, Specular, 0.25f);
        DEFINE_MEMBER(float, Shininess, 4.0f);

        DEFINE_MEMBER(GLuint, VertexArray, 0);
        DEFINE_MEMBER(GLuint, VertexBuffer, 0);

        QOpenGLTexture* mTexture;
        QVector<Vertex> mVertices;
    };
}
