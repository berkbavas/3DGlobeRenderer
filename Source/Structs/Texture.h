#pragma once

#include <QOpenGLContext>
#include <QString>

namespace GlobeRenderer
{
    struct Texture
    {
        GLuint id;     // OpenGL handle
        GLuint target; // GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP etc.
        GLuint unit;   // Uniform location
        QString name;  // Texture name in the shader
    };
}