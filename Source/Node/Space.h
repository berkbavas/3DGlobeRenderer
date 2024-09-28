#pragma once

#include "Node/ModelData.h"
#include "Util/Macros.h"

#include <QObject>
#include <QOpenGLExtraFunctions>

namespace GlobeRenderer
{
    class Space : public QObject, protected QOpenGLExtraFunctions
    {
      public:
        explicit Space(QObject* parent = nullptr);

        void LoadModelData(const QString& path);
        void LoadTextures(const QString& folder, const QString& extension);
        void Render();

      private:
        DEFINE_MEMBER(GLuint, VertexArray, 0);
        DEFINE_MEMBER(GLuint, VertexBuffer, 0);
        DEFINE_MEMBER(float, Brightness, 1.0f);

        ModelData* mModelData;
        GLuint mTextureId;
    };
}