#pragma once

#include "Node/ModelData.h"
#include "Node/Node.h"
#include "Node/Sphere.h"
#include "Util/Macros.h"
#include "Util/TextureLoader.h"

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

        void Render();
        void BindTextures();
        void ReleaseTextures();
        void AddTexture(GLuint unit, const QString& path);
        void AddTextureCubeMap(GLuint unit, const QString& folder, const QString& extension);

        void Rotate(const QVector3D& axis, float angle);

      private:
        DEFINE_MEMBER(float, Ambient, 1.0f);
        DEFINE_MEMBER(float, Diffuse, 1.0f);
        DEFINE_MEMBER(float, Specular, 0.25f);
        DEFINE_MEMBER(float, Shininess, 4.0f);

        std::map<GLuint, GLuint> mTextures;
        std::map<GLuint, GLuint> mTextureTargets;
        Sphere* mSphere;

        TextureLoader* mTextureLoader;
    };
}
