#include "Sun.h"

#include <QtMath>

#include <imgui.h>
#include <QtImGui.h>

Sun::Sun()
    : mDirection(1, 0, 0)
    , mColor(1, 1, 1, 1)
    , mAmbient(1.0)
    , mDiffuse(0.75)
    , mSpecular(0.25)

{}

void Sun::drawGUI()
{
    ImGui::SetNextWindowSize(ImVec2(420, 820), ImGuiCond_FirstUseEver);
    ImGui::Begin("Sun");

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Direction:");
    float x = mDirection.x();
    float y = mDirection.y();
    float z = mDirection.z();
    float r = mDirection.length();
    float theta = qRadiansToDegrees(atan2(z, x));
    float phi = qRadiansToDegrees(atan2(y, sqrt(z * z + x * x)));

    if (qFuzzyCompare(abs(phi), 90.0f))
        theta = 0.0f;

    bool updated = false;

    if (ImGui::SliderFloat("Theta##Sun", &theta, -180.0f, 180.0f, "%.1f"))
        updated = true;

    if (ImGui::SliderFloat("Phi##Sun", &phi, -90.0f, 90.0f, "%.1f"))
        updated = true;

    if (updated)
    {
        x = r * cos(qDegreesToRadians(phi)) * cos(qDegreesToRadians(theta));
        y = r * sin(qDegreesToRadians(phi));
        z = r * cos(qDegreesToRadians(phi)) * sin(qDegreesToRadians(theta));

        mDirection = QVector3D(x, y, z);
    }

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Shading Parameters:");
    ImGui::SliderFloat("Ambient##Sun", &mAmbient, 0.0f, 1.0f, "%.3f");
    ImGui::SliderFloat("Diffuse##Sun", &mDiffuse, 0.0f, 1.0f, "%.3f");
    ImGui::SliderFloat("Specular##Sun", &mSpecular, 0.0f, 1.0f, "%.3f");

    float colorf[4] = {mColor.x(), mColor.y(), mColor.z(), mColor.w()};

    if (ImGui::ColorEdit4("Color##Sun", (float *) &colorf))
        mColor = QVector4D(colorf[0], colorf[1], colorf[2], colorf[3]);

    ImGui::End();
}

Sun *Sun::instance()
{
    static Sun instance;

    return &instance;
}

const QVector3D &Sun::direction() const
{
    return mDirection;
}

void Sun::setDirection(const QVector3D &newDirection)
{
    mDirection = newDirection;
}

const QVector4D &Sun::color() const
{
    return mColor;
}

void Sun::setColor(const QVector4D &newColor)
{
    mColor = newColor;
}

float Sun::ambient() const
{
    return mAmbient;
}

void Sun::setAmbient(float newAmbient)
{
    mAmbient = newAmbient;
}

float Sun::diffuse() const
{
    return mDiffuse;
}

void Sun::setDiffuse(float newDiffuse)
{
    mDiffuse = newDiffuse;
}

float Sun::specular() const
{
    return mSpecular;
}

void Sun::setSpecular(float newSpecular)
{
    mSpecular = newSpecular;
}
