#pragma once

#include "Node/ModelData.h"
#include "Node/Node.h"
#include "Util/Macros.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QVector>
#include <map>

namespace GlobeRenderer
{
    class Globe : public Node, protected QOpenGLExtraFunctions
    {
      public:
        explicit Globe(QObject* parent = nullptr);
        ~Globe() = default;

        void LoadModelData(const QString& path);
        void Render();
        void BindTextures();
        void ReleaseTextures();
        void AddTexture(GLuint unit, const QString& path);

        void Rotate(const QVector3D& axis, float angle);

      private:
        DEFINE_MEMBER(float, Ambient, 1.0f);
        DEFINE_MEMBER(float, Diffuse, 1.0f);
        DEFINE_MEMBER(float, Specular, 0.25f);
        DEFINE_MEMBER(float, Shininess, 4.0f);

        std::map<GLuint, QOpenGLTexture*> mTextures;
        ModelData* mModelData;
    };
}
