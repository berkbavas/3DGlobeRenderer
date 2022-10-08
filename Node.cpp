#include "Node.h"
#include "Helper.h"

#include <QtMath>

Node::Node(QObject *parent)
    : QObject{parent}
    , mPosition(0, 0, 0)
    , mScale(1, 1, 1)
    , mNodeType(NodeType::DummyNode)

{}

Node::~Node() {}

const QVector3D &Node::position() const
{
    return mPosition;
}

void Node::setPosition(const QVector3D &newPosition)
{
    mPosition = newPosition;
}

const QQuaternion &Node::rotation() const
{
    return mRotation;
}

void Node::setRotation(const QQuaternion &newRotation)
{
    mRotation = newRotation;
}

const QVector3D &Node::scale() const
{
    return mScale;
}

void Node::setScale(const QVector3D &newScale)
{
    mScale = newScale;
}

QMatrix4x4 Node::transformation() const
{
    QMatrix4x4 transformation;
    transformation.scale(mScale);
    transformation.rotate(mRotation);
    transformation.setColumn(3, QVector4D(mPosition, 1.0f));

    return transformation;
}

const QString &Node::name() const
{
    return mName;
}

void Node::setName(const QString &newName)
{
    mName = newName;
}

Node::NodeType Node::nodeType() const
{
    return mNodeType;
}

QString Node::nodeTypeString()
{
    switch (mNodeType)
    {
    case NodeType::DummyNode:
        return "DummyNode";
    case NodeType::Model:
        return "Model";
    case NodeType::FreeCamera:
        return "Free Camera";
    case NodeType::DummyCamera:
        return "Dummy Camera";
    default:
        return "Unknown Node";
    }
}

QMatrix3x3 Node::normalMatrix() const
{
    return transformation().normalMatrix();
}

void Node::drawGUI()
{
    // Position
    if (!ImGui::CollapsingHeader("Position##Node"))
    {
        float x = mPosition.x();
        float y = mPosition.y();
        float z = mPosition.z();

        if (ImGui::DragFloat("x##NodePosition", &x, 0.01f, -1000.0f, 1000.0f, "%.3f"))
            mPosition = QVector3D(x, y, z);
        if (ImGui::DragFloat("y##NodePosition", &y, 0.01f, -1000.0f, 1000.0f, "%.3f"))
            mPosition = QVector3D(x, y, z);
        if (ImGui::DragFloat("z##NodePosition", &z, 0.01f, -1000.0f, 1000.0f, "%.3f"))
            mPosition = QVector3D(x, y, z);
    }

    // Rotation
    if (ImGui::CollapsingHeader("Rotation##Node"))
    {
        float yaw, pitch, roll;

        Helper::getEulerDegrees(mRotation, yaw, pitch, roll);

        if (ImGui::SliderFloat("Yaw##NodeRotation", &yaw, 0.0f, 359.999f, "%.3f"))
            mRotation = Helper::constructFromEulerDegrees(yaw, pitch, roll);
        if (ImGui::SliderFloat("Pitch##NodeRotation", &pitch, -89.999f, 89.999f, "%.3f"))
            mRotation = Helper::constructFromEulerDegrees(yaw, pitch, roll);
        if (ImGui::SliderFloat("Roll##NodeRotation", &roll, -179.999f, 179.999f, "%.3f"))
            mRotation = Helper::constructFromEulerDegrees(yaw, pitch, roll);
    }

    // Scale
    if (ImGui::CollapsingHeader("Scale##Node"))
    {
        QVector3D scale = mScale;
        float x = scale.x();
        float y = scale.y();
        float z = scale.z();
        float all = qMin(x, qMin(y, z));

        if (ImGui::DragFloat("All##NodeScale", &all, 0.01f, 0.0001f, 100.0f, "%.3f"))
            mScale = QVector3D(all, all, all);
        if (ImGui::DragFloat("x##NodeScale", &x, 0.01f, 0.0001f, 100.0f, "%.3f"))
            mScale = QVector3D(x, y, z);
        if (ImGui::DragFloat("y##NodeScale", &y, 0.01f, 0.0001f, 100.0f, "%.3f"))
            mScale = QVector3D(x, y, z);
        if (ImGui::DragFloat("z##NodeScale", &z, 0.01f, 0.0001f, 100.0f, "%.3f"))
            mScale = QVector3D(x, y, z);
    }
}
