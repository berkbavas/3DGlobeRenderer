#pragma once

#include "Util/Macros.h"

#include <QMatrix4x4>
#include <QQuaternion>
#include <QSharedPointer>
#include <QVector3D>
#include <QtMath>

namespace GlobeRenderer
{
    class Node
    {
        DISABLE_COPY(Node);

      protected:
        Node() = default;
        virtual ~Node() = default;

      public:
        const QMatrix4x4& GetTransformation() const { return mTransformation; }
        const QQuaternion& GetRotation() const { return mRotation; }

        const QVector3D& GetPosition() const { return mPosition; }
        const float GetPosition(int coord) const { return mPosition[coord]; }
        float& GetPosition(int coord) { return mPosition[coord]; }

        const QVector3D& GetScale() const { return mScale; }
        const float GetScale(int coord) const { return mScale[coord]; }
        float& GetScale(int coord) { return mScale[coord]; }

        void SetTransformation(const QMatrix4x4& newTransformation);
        void SetRotation(const QQuaternion& newRotation);
        void SetPosition(const QVector3D& newPosition);
        void SetPosition(float x, float y, float z);
        void SetScale(const QVector3D& newScale);
        void SetScale(float x, float y, float z);

        virtual void UpdateTransformation();

        void RotateGlobal(const QVector3D& axis, float angle);
        void RotateLocal(const QVector3D& axis, float angle);
        void Translate(const QVector3D& delta);

      private:
        QMatrix4x4 mTransformation;
        QQuaternion mRotation;
        QVector3D mPosition{ QVector3D(0, 0, 0) };
        QVector3D mScale{ QVector3D(1, 1, 1) };

        DEFINE_MEMBER(QString, NodeName);
    };

    using NodePtr = QSharedPointer<Node>;
}
