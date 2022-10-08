#include "FreeCamera.h"
#include "Helper.h"

#include <QTransform>
#include <QtMath>

FreeCamera::FreeCamera(QObject *parent)
    : PerspectiveCamera(parent)
    , mUpdateRotation(true)
    , mUpdatePosition(true)

{
    mNodeType = Node::NodeType::FreeCamera;

    connect(this, &Camera::activeChanged, this, [=](bool active) {
        if (!active)
            reset();
    });

    mMouse.pressed = false;
    mMouse.x = 0;
    mMouse.y = 0;
    mMouse.dx = 0;
    mMouse.dy = 0;

    mSpeed.movement = 5.0f;
    mSpeed.angular = 25.0f;
    mSpeed.movementMultiplier = 1.0f;
    mSpeed.angularMultiplier = 1.0f;
}

FreeCamera::~FreeCamera() {}

void FreeCamera::mouseDoubleClicked(QMouseEvent *) {}

void FreeCamera::wheelMoved(QWheelEvent *) {}

void FreeCamera::mousePressed(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        mMouse.x = event->position().x();
        mMouse.y = event->position().y();
        mMouse.pressed = true;
    }
}

void FreeCamera::mouseReleased(QMouseEvent *)
{
    mMouse.pressed = false;
}

void FreeCamera::mouseMoved(QMouseEvent *event)
{
    if (mMouse.pressed)
    {
        mMouse.dx += mMouse.x - event->position().x();
        mMouse.dy += mMouse.y - event->position().y();

        mMouse.x = event->position().x();
        mMouse.y = event->position().y();
        mUpdateRotation = true;
    }
}

void FreeCamera::keyPressed(QKeyEvent *event)
{
    mPressedKeys.insert((Qt::Key) event->key(), true);
    mUpdatePosition = true;
}

void FreeCamera::keyReleased(QKeyEvent *event)
{
    mPressedKeys.insert((Qt::Key) event->key(), false);
}

void FreeCamera::update(float ifps)
{
    // Rotation
    if (mUpdateRotation)
    {
        mRotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), mSpeed.angularMultiplier * mSpeed.angular * mMouse.dx * ifps) * mRotation;
        mRotation = mRotation * QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), mSpeed.angularMultiplier * mSpeed.angular * mMouse.dy * ifps);

        mMouse.dx = 0.0f;
        mMouse.dy = 0.0f;
        mUpdateRotation = false;
    }

    // Translation
    if (mUpdatePosition)
    {
        const QList<Qt::Key> keys = mPressedKeys.keys();

        if (mPressedKeys[Qt::Key_Control])
            mSpeed.movement = 100.0f;
        else if (mPressedKeys[Qt::Key_Shift])
            mSpeed.movement = 20.0f;
        else
            mSpeed.movement = 5.0f;

        for (auto key : keys)
            if (mPressedKeys.value(key, false))
                mPosition += mSpeed.movementMultiplier * mSpeed.movement * ifps * mRotation.rotatedVector(KEY_BINDINGS.value(key, QVector3D(0, 0, 0)));
    }

    if (mPressedKeys.empty())
        mUpdatePosition = false;
}

void FreeCamera::reset()
{
    auto keys = mPressedKeys.keys();
    for (auto key : qAsConst(keys))
        mPressedKeys.insert(key, false);

    mMouse.pressed = false;
}

void FreeCamera::drawGUI()
{
    if (!ImGui::CollapsingHeader("Parameters##FreeCamera"))
    {
        ImGui::SliderFloat("Movement Speed##FreeCamera", &mSpeed.movementMultiplier, 0, 100);
        ImGui::SliderFloat("Angular Speed##FreeCamera", &mSpeed.angularMultiplier, 0, 5);
        if (ImGui::SliderFloat("FOV##ProjectionParameters", &mVerticalFov, 1.0f, 120.0))
            setVerticalFov(mVerticalFov);
        ImGui::SliderFloat("Z-Near##ProjectionParameters", &mZNear, 0.1f, 100.0f);
        ImGui::SliderFloat("Z-Far##ProjectionParameters", &mZFar, 1000.0f, 1000000.0f);
    }

    if (!ImGui::CollapsingHeader("Position##FreeCamera"))
    {
        float x = mPosition.x();
        float y = mPosition.y();
        float z = mPosition.z();

        if (ImGui::InputFloat("x##FreeCameraWorldPosition", &x, 0.01f, 10.0f, "%.3f"))
            setPosition(QVector3D(x, y, z));
        if (ImGui::InputFloat("y##FreeCameraWorldPosition", &y, 0.01f, 10.0f, "%.3f"))
            setPosition(QVector3D(x, y, z));
        if (ImGui::InputFloat("z##FreeCameraWorldPosition", &z, 0.01f, 10.0f, "%.3f"))
            setPosition(QVector3D(x, y, z));
    }

    // World rotation
    if (!ImGui::CollapsingHeader("World Rotation##FreeCamera"))
    {
        float yaw, pitch, roll;

        Helper::getEulerDegrees(mRotation, yaw, pitch, roll);

        if (ImGui::SliderFloat("Yaw##FreeCamera", &yaw, 0.0f, 359.999f, "%.3f"))
            setRotation(Helper::constructFromEulerDegrees(yaw, pitch, roll));
        if (ImGui::SliderFloat("Pitch##FreeCamera", &pitch, -89.999f, 89.999f, "%.3f"))
            setRotation(Helper::constructFromEulerDegrees(yaw, pitch, roll));
        if (ImGui::SliderFloat("Roll##FreeCamera", &roll, -179.999f, 179.999f, "%.3f"))
            setRotation(Helper::constructFromEulerDegrees(yaw, pitch, roll));
    }
}

const QMap<Qt::Key, QVector3D> FreeCamera::KEY_BINDINGS = {
    {Qt::Key_W, QVector3D(0, 0, -1)},
    {Qt::Key_S, QVector3D(0, 0, 1)},
    {Qt::Key_A, QVector3D(-1, 0, 0)},
    {Qt::Key_D, QVector3D(1, 0, 0)},
    {Qt::Key_E, QVector3D(0, 1, 0)},
    {Qt::Key_Q, QVector3D(0, -1, 0)},
};
