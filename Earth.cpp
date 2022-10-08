#include "Earth.h"
#include "Helper.h"

#include <QImageReader>

Earth::Earth(QObject *parent)
    : Model("Earth", parent)
{
    QImageReader::setAllocationLimit(0);

    // Texture
    {
        QImage image("Resources/Textures/world.200409.3x21600x10800.jpg");
        image.mirror();
        mTexture = new QOpenGLTexture(image);
        mTexture->setWrapMode(QOpenGLTexture::WrapMode::Repeat);
        mTexture->setMinMagFilters(QOpenGLTexture::Filter::LinearMipMapLinear, QOpenGLTexture::Filter::Linear);
    }

    // Height Map
    {
        QImage image("Resources/HeightMaps/gebco_08_rev_elev_21600x10800.png");
        image.mirror();
        mHeightMap = new QOpenGLTexture(image);
        mHeightMap->setWrapMode(QOpenGLTexture::WrapMode::Repeat);
        mHeightMap->setMinMagFilters(QOpenGLTexture::Filter::LinearMipMapLinear, QOpenGLTexture::Filter::Linear);
    }

    mAmbient = 1.0f;
}

Earth *Earth::instance()
{
    static Earth instance;
    return &instance;
}

QOpenGLTexture *Earth::texture() const
{
    return mTexture;
}

QOpenGLTexture *Earth::heightMap() const
{
    return mHeightMap;
}

void Earth::drawGUI()
{
    ImGui::SetNextWindowSize(ImVec2(420, 820), ImGuiCond_FirstUseEver);
    ImGui::Begin("Earth");

    // Shading
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Shading Parameters:");
        ImGui::SliderFloat("Ambient##Model", &mAmbient, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Diffuse##Model", &mDiffuse, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Specular##Model", &mSpecular, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Shininess##Model", &mShininess, 1.0f, 128.0f, "%.3f");
        ImGui::ColorEdit4("Color##Model", (float *) &mColor);
    }

    // Position
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Position:");

        float x = mPosition.x();
        float y = mPosition.y();
        float z = mPosition.z();

        if (ImGui::DragFloat("x##EarthPosition", &x, 0.01f, -1000.0f, 1000.0f, "%.3f"))
            mPosition = QVector3D(x, y, z);
        if (ImGui::DragFloat("y##EarthPosition", &y, 0.01f, -1000.0f, 1000.0f, "%.3f"))
            mPosition = QVector3D(x, y, z);
        if (ImGui::DragFloat("z##EarthPosition", &z, 0.01f, -1000.0f, 1000.0f, "%.3f"))
            mPosition = QVector3D(x, y, z);
    }

    // Rotation
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Rotation:");
        float yaw, pitch, roll;
        Helper::getEulerDegrees(mRotation, yaw, pitch, roll);

        if (ImGui::SliderFloat("Yaw##EarthRotation", &yaw, 0.0f, 359.999f, "%.3f"))
            mRotation = Helper::constructFromEulerDegrees(yaw, pitch, roll);
        if (ImGui::SliderFloat("Pitch##EarthRotation", &pitch, -89.999f, 89.999f, "%.3f"))
            mRotation = Helper::constructFromEulerDegrees(yaw, pitch, roll);
        if (ImGui::SliderFloat("Roll##EarthRotation", &roll, -179.999f, 179.999f, "%.3f"))
            mRotation = Helper::constructFromEulerDegrees(yaw, pitch, roll);
    }

    // Scale
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Scale:");

        float x = mScale.x();
        float y = mScale.y();
        float z = mScale.z();
        float all = qMin(x, qMin(y, z));

        if (ImGui::DragFloat("All##EarthScale", &all, 0.01f, 0.0001f, 100.0f, "%.3f"))
            mScale = QVector3D(all, all, all);
        if (ImGui::DragFloat("x##EarthScale", &x, 0.01f, 0.0001f, 100.0f, "%.3f"))
            mScale = QVector3D(x, y, z);
        if (ImGui::DragFloat("y##EarthScale", &y, 0.01f, 0.0001f, 100.0f, "%.3f"))
            mScale = QVector3D(x, y, z);
        if (ImGui::DragFloat("z##EarthScale", &z, 0.01f, 0.0001f, 100.0f, "%.3f"))
            mScale = QVector3D(x, y, z);
    }

    ImGui::End();
}
