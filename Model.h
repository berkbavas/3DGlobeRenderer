#ifndef MODEL_H
#define MODEL_H

#include "Node.h"

class Model : public Node
{
public:
    explicit Model(const QString &modelName, QObject *parent = nullptr);
    virtual ~Model();

    void drawGUI() override;

    const QVector4D &color() const;
    void setColor(const QVector4D &newColor);

    float ambient() const;
    void setAmbient(float newAmbient);

    float diffuse() const;
    void setDiffuse(float newDiffuse);

    float specular() const;
    void setSpecular(float newSpecular);

    float shininess() const;
    void setShininess(float newShininess);

protected:
    QVector4D mColor;
    float mAmbient;
    float mDiffuse;
    float mSpecular;
    float mShininess;
};

#endif // MODEL_H
