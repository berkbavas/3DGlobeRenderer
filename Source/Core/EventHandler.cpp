#include "EventHandler.h"

#include "Renderer/Renderer.h"
#include "Util/Math.h"
#include "Util/Util.h"

#include <QtMath>

EarthRenderer::EventHandler::EventHandler(QObject* parent)
    : QObject(parent)
{
}
void EarthRenderer::EventHandler::Update(float ifps)
{
    UpdateCameraTransformation(ifps);
    UpdateEarthTransformation(ifps);
}

void EarthRenderer::EventHandler::DrawGui()
{
    if (!ImGui::CollapsingHeader("Action Speed"))
    {
        ImGui::SliderFloat("Camera Tilt Speed", &mCameraTiltSpeed, 1.0f, 50.0f, "%.2f");
        ImGui::SliderFloat("Camera Distance Speed", &mCameraDistanceSpeed, 1.0f, 50.0f, "%.2f");
        ImGui::SliderFloat("Earth Rotation Speed", &mEarthRotationSpeed, 1.0f, 50.0f, "%.2f");
    }
}

void EarthRenderer::EventHandler::UpdateCameraTransformation(float ifps)
{
    if (!Util::IsBetween(-0.1f, mDistanceToBeConsumed, 0.1f))
    {
        const auto distance = mCameraDistanceSpeed * mDistanceToBeConsumed * ifps;
        mDistanceToBeConsumed -= distance;
        mCamera->AddDistance(distance);
    }
    else
    {
        mDistanceToBeConsumed = 0;
    }

    if (!Util::IsBetween(-0.1f, mTiltAngleToBeConsumed, 0.1f))
    {
        const auto angle = mCameraTiltSpeed * mTiltAngleToBeConsumed * ifps;
        mTiltAngleToBeConsumed -= angle;
        mCamera->AddTilt(angle);
    }
    else
    {
        mTiltAngleToBeConsumed = 0;
    }
}

void EarthRenderer::EventHandler::UpdateEarthTransformation(float ifps)
{
    if (!Util::IsBetween(-0.1f, mEarthRotationAngleToBeConsumed, 0.1f))
    {
        const auto angle = mEarthRotationSpeed * mEarthRotationAngleToBeConsumed * ifps;
        mEarthRotationAngleToBeConsumed -= angle;
        mEarth->Rotate(mRotationAxis, angle);
    }
}

void EarthRenderer::EventHandler::MousePressed(QMouseEvent* event)
{
    const auto x = event->pos().x() * mDevicePixelRatio;
    const auto y = event->pos().y() * mDevicePixelRatio;

    mPressedButton = event->button();

    if (mPressedButton == Qt::LeftButton)
    {
        mMouseEarthPosition = mRenderer->GetMouseWorldPosition(x, y);
        mEarthRotationAngleToBeConsumed = 0;
    }

    if (mPressedButton == Qt::RightButton)
    {
        mMouse.y = event->pos().y();
    }
}

void EarthRenderer::EventHandler::MouseReleased(QMouseEvent* event)
{
    mPressedButton = Qt::NoButton;
}

void EarthRenderer::EventHandler::MouseMoved(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    const auto x = event->pos().x() * mDevicePixelRatio;
    const auto y = event->pos().y() * mDevicePixelRatio;

    if (mPressedButton == Qt::LeftButton)
    {

        QVector3D p0 = mMouseEarthPosition;
        QVector3D p1 = mRenderer->GetMouseWorldPosition(x, y);
        QVector3D v0 = p0.normalized();
        QVector3D v1 = p1.normalized();

        mRotationAxis = QVector3D::crossProduct(p0, p1);
        mEarthRotationAngleToBeConsumed += Math::AngleBetween(p0, p1);

        mMouseEarthPosition = p1;
    }

    if (mPressedButton == Qt::RightButton)
    {
        mTiltAngleToBeConsumed += 0.05 * (mMouse.y - y);
        mMouse.y = y;
    }
}

void EarthRenderer::EventHandler::WheelMoved(QWheelEvent* event)
{
    mDistanceToBeConsumed += Util::Sign(event->angleDelta().y());
}

void EarthRenderer::EventHandler::SetRenderer(Renderer* renderer)
{
    mRenderer = renderer;
}

void EarthRenderer::EventHandler::Initialize()
{
    mCamera = mRenderer->GetCamera();
    mEarth = mRenderer->GetEarth();
}
