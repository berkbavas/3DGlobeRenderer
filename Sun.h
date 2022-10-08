#ifndef SUN_H
#define SUN_H

#include <QVector4D>

class Sun
{
private:
    Sun();

public:
    void drawGUI();

    static Sun *instance();

    const QVector3D &direction() const;
    void setDirection(const QVector3D &newDirection);

    const QVector4D &color() const;
    void setColor(const QVector4D &newColor);

    float ambient() const;
    void setAmbient(float newAmbient);

    float diffuse() const;
    void setDiffuse(float newDiffuse);

    float specular() const;
    void setSpecular(float newSpecular);

private:
    QVector3D mDirection;
    QVector4D mColor;
    float mAmbient;
    float mDiffuse;
    float mSpecular;
};

#endif // SUN_H
