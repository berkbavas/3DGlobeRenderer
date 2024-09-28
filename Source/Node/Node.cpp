#include "Node.h"

#include <QtMath>

GlobeRenderer::Node::Node(QObject* parent)
    : QObject(parent)
    , mPosition(0, 0, 0)
    , mScale(1, 1, 1)
{
}

const QQuaternion& GlobeRenderer::Node::GetRotation() const
{
    return mRotation;
}

void GlobeRenderer::Node::SetRotation(const QQuaternion& newRotation)
{
    if (mRotation == newRotation)
    {
        return;
    }

    mRotation = newRotation;
    UpdateTransformation();
}

const QVector3D& GlobeRenderer::Node::GetPosition() const
{
    return mPosition;
}

QVector3D& GlobeRenderer::Node::GetPosition()
{
    return mPosition;
}

void GlobeRenderer::Node::SetPosition(const QVector3D& newPosition)
{
    if (mPosition == newPosition)
    {
        return;
    }

    mPosition = newPosition;
    UpdateTransformation();
}

const QVector3D& GlobeRenderer::Node::GetScale() const
{
    return mScale;
}

void GlobeRenderer::Node::SetScale(const QVector3D& newScale)
{
    if (mScale == newScale)
    {
        return;
    }

    mScale = newScale;
    UpdateTransformation();
}

const QMatrix4x4& GlobeRenderer::Node::GetTransformation() const
{
    return mTransformation;
}

void GlobeRenderer::Node::SetTransformation(const QMatrix4x4& newTransformation)
{
    if (mTransformation == newTransformation)
    {
        return;
    }

    mTransformation = newTransformation;
    mPosition = mTransformation.column(3).toVector3D();
    mRotation = QQuaternion::fromRotationMatrix(mTransformation.normalMatrix());
}

void GlobeRenderer::Node::UpdateTransformation()
{
    mTransformation.setToIdentity();
    mTransformation.scale(mScale);
    mTransformation.rotate(mRotation);
    mTransformation.setColumn(3, QVector4D(mPosition, 1.0f));
}

void GlobeRenderer::Node::SetPosition(float x, float y, float z)
{
    SetPosition(QVector3D(x, y, z));
}

void GlobeRenderer::Node::SetScale(float x, float y, float z)
{
    SetScale(QVector3D(x, y, z));
}
QVector3D& GlobeRenderer::Node::GetScale()
{
    return mScale;
}