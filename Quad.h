#ifndef QUAD_H
#define QUAD_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

class Quad : protected QOpenGLExtraFunctions
{
private:
    explicit Quad();

public:
    void render();

    static Quad *instance();

private:
    unsigned int mVAO;
    unsigned int mVBO;
    static const float VERTICES[24];
};

#endif // QUAD_H
