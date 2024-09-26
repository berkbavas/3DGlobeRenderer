#include "Node.h"

#include <QtMath>

EarthRenderer::Node::Node(QObject* parent)
    : QObject(parent)
    , mPosition(0, 0, 0)
    , mScale(1, 1, 1)
{
}

const QQuaternion& EarthRenderer::Node::GetRotation() const
{
    return mRotation;
}

void EarthRenderer::Node::SetRotation(const QQuaternion& newRotation)
{
    if (mRotation == newRotation)
    {
        return;
    }

    mRotation = newRotation;
    UpdateTransformation();
}

const QVector3D& EarthRenderer::Node::GetPosition() const
{
    return mPosition;
}

QVector3D& EarthRenderer::Node::GetPosition()
{
    return mPosition;
}

void EarthRenderer::Node::SetPosition(const QVector3D& newPosition)
{
    if (mPosition == newPosition)
    {
        return;
    }

    mPosition = newPosition;
    UpdateTransformation();
}

const QVector3D& EarthRenderer::Node::GetScale() const
{
    return mScale;
}

void EarthRenderer::Node::SetScale(const QVector3D& newScale)
{
    if (mScale == newScale)
    {
        return;
    }

    mScale = newScale;
    UpdateTransformation();
}

const QMatrix4x4& EarthRenderer::Node::GetTransformation() const
{
    return mTransformation;
}

void EarthRenderer::Node::SetTransformation(const QMatrix4x4& newTransformation)
{
    if (mTransformation == newTransformation)
    {
        return;
    }

    mTransformation = newTransformation;
    mPosition = mTransformation.column(3).toVector3D();
    mRotation = QQuaternion::fromRotationMatrix(mTransformation.normalMatrix());
}

void EarthRenderer::Node::UpdateTransformation()
{
    mTransformation.setToIdentity();
    mTransformation.scale(mScale);
    mTransformation.rotate(mRotation);
    mTransformation.setColumn(3, QVector4D(mPosition, 1.0f));
}

void EarthRenderer::Node::SetPosition(float x, float y, float z)
{
    SetPosition(QVector3D(x, y, z));
}

void EarthRenderer::Node::SetScale(float x, float y, float z)
{
    SetScale(QVector3D(x, y, z));
}
QVector3D& EarthRenderer::Node::GetScale()
{
    return mScale;
}