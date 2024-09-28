#pragma once

#include "Util/Macros.h"

#include <QMatrix4x4>
#include <QObject>
#include <QQuaternion>
#include <QVector3D>
#include <QtImGui.h>
#include <imgui.h>

namespace GlobeRenderer
{
    class Node : public QObject
    {
      protected:
        friend class OpenGLWidget;

        explicit Node(QObject* parent = nullptr);
        virtual ~Node() = default;

      public:
        const QMatrix4x4& GetTransformation() const;
        void SetTransformation(const QMatrix4x4& newTransformation);

        const QQuaternion& GetRotation() const;
        void SetRotation(const QQuaternion& newRotation);

        const QVector3D& GetPosition() const;
        QVector3D& GetPosition();
        void SetPosition(const QVector3D& newPosition);

        const QVector3D& GetScale() const;
        QVector3D& GetScale();
        void SetScale(const QVector3D& newScale);

        void SetPosition(float x, float y, float z);
        void SetScale(float x, float y, float z);

        virtual void UpdateTransformation();

      private:
        QMatrix4x4 mTransformation;
        QQuaternion mRotation;
        QVector3D mPosition;
        QVector3D mScale;
    };
}
