#pragma once

#include <QOpenGLContext>
#include <QString>

namespace GlobeRenderer
{
    struct Texture
    {
        GLuint Id;     // OpenGL handle
        GLuint Target; // GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP etc.
        GLuint Unit;   // 0, 1, 2, ... (GL_TEXTURE0 + Unit)
        QString Name;  // Texture name in the shader
    };
}