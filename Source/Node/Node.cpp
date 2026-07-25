
#include "Node.h"

void GlobeRenderer::Node::SetRotation(const QQuaternion& NewRotation)
{
    mRotation = NewRotation;
    UpdateTransformation();
}

void GlobeRenderer::Node::SetPosition(const QVector3D& NewPosition)
{
    mPosition = NewPosition;
    UpdateTransformation();
}

void GlobeRenderer::Node::SetScale(const QVector3D& NewScale)
{
    mScale = NewScale;
    UpdateTransformation();
}

const QMatrix4x4& GlobeRenderer::Node::GetTransformation() const
{
    return mTransformation;
}

void GlobeRenderer::Node::SetTransformation(const QMatrix4x4& NewTransformation)
{
    // Decompose the matrix into position, rotation, and scale.
    mTransformation = NewTransformation;
    mPosition = mTransformation.column(3).toVector3D();
    mScale = QVector3D(mTransformation.column(0).length(), mTransformation.column(1).length(), mTransformation.column(2).length());
    QMatrix4x4 RotationMatrix = mTransformation;
    RotationMatrix.setColumn(0, RotationMatrix.column(0) / mScale.x());
    RotationMatrix.setColumn(1, RotationMatrix.column(1) / mScale.y());
    RotationMatrix.setColumn(2, RotationMatrix.column(2) / mScale.z());
    mRotation = QQuaternion::fromRotationMatrix(RotationMatrix.toGenericMatrix<3, 3>());
    mNormalMatrix = mTransformation.normalMatrix();
}

void GlobeRenderer::Node::SetPosition(float X, float Y, float Z)
{
    SetPosition(QVector3D(X, Y, Z));
}

void GlobeRenderer::Node::SetScale(float X, float Y, float Z)
{
    SetScale(QVector3D(X, Y, Z));
}

void GlobeRenderer::Node::SetScale(float UniformScale)
{
    SetScale(UniformScale, UniformScale, UniformScale);
}

void GlobeRenderer::Node::RotateGlobal(const QVector3D& Axis, float Angle)
{
    SetRotation(QQuaternion::fromAxisAndAngle(Axis, Angle) * GetRotation());
}

void GlobeRenderer::Node::RotateLocal(const QVector3D& Axis, float Angle)
{
    SetRotation(GetRotation() * QQuaternion::fromAxisAndAngle(Axis, Angle));
}

void GlobeRenderer::Node::Translate(const QVector3D& Delta)
{
    SetPosition(GetPosition() + Delta);
}

void GlobeRenderer::Node::UpdateTransformation()
{
    // Rebuild the transformation matrix from position, rotation, and scale.
    // Note: The order of transformations is important.
    // We scale first, then rotate, then translate.
    // But reverse order of operations is applied when multiplying matrices.
    // For example, if you want to scale an object, then rotate it, and finally translate it,
    // you would multiply the matrices in the order: Translation * Rotation * Scale.
    // Translation is applied last, so it should be the first matrix in the multiplication order.

    mTransformation.setToIdentity();
    mTransformation.translate(mPosition);
    mTransformation.rotate(mRotation);
    mTransformation.scale(mScale);
    mNormalMatrix = mTransformation.normalMatrix();
}

const QQuaternion& GlobeRenderer::Node::GetRotation() const
{
    return mRotation;
}

const QVector3D& GlobeRenderer::Node::GetPosition() const
{
    return mPosition;
}

const QVector3D& GlobeRenderer::Node::GetScale() const
{
    return mScale;
}

const QMatrix3x3& GlobeRenderer::Node::GetNormalMatrix() const
{
    return mNormalMatrix;
}
