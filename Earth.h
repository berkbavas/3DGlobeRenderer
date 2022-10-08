#ifndef EARTH_H
#define EARTH_H

#include "Model.h"

#include <QOpenGLTexture>

class Earth : public Model
{
private:
    explicit Earth(QObject *parent = nullptr);

public:
    static Earth *instance();

    QOpenGLTexture *texture() const;
    QOpenGLTexture *heightMap() const;

    void drawGUI() override;

private:
    QOpenGLTexture *mTexture;
    QOpenGLTexture *mHeightMap;
};

#endif // EARTH_H
