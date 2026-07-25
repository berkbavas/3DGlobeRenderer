#include "Cube.h"

GlobeRenderer::Cube::Cube()
{
    initializeOpenGLFunctions();

    const auto Vertices = CreateVertices();
    mNumberOfVertices = Vertices.size();

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(QVector3D), Vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

GlobeRenderer::Cube::~Cube()
{
    if (mVAO)
    {
        glDeleteVertexArrays(1, &mVAO);
        mVAO = 0;
    }

    if (mVBO)
    {
        glDeleteBuffers(1, &mVBO);
        mVBO = 0;
    }
}

void GlobeRenderer::Cube::Render()
{
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);
    glBindVertexArray(0);
}

QVector<QVector3D> GlobeRenderer::Cube::CreateVertices()
{
    // Create a cube with 36 vertices (6 faces * 2 triangles * 3 vertices)
    QVector<QVector3D> Vertices;
    Vertices.reserve(36);

    // Front face
    Vertices.append(QVector3D(-1.0f, -1.0f, 1.0f));
    Vertices.append(QVector3D(1.0f, -1.0f, 1.0f));
    Vertices.append(QVector3D(1.0f, 1.0f, 1.0f));
    Vertices.append(QVector3D(1.0f, 1.0f, 1.0f));
    Vertices.append(QVector3D(-1.0f, 1.0f, 1.0f));
    Vertices.append(QVector3D(-1.0f, -1.0f, 1.0f));

    // Back face
    Vertices.append(QVector3D(-1.0f, -1.0f, -1.0f));
    Vertices.append(QVector3D(-1.0f, 1.0f, -1.0f));
    Vertices.append(QVector3D(1.0f, 1.0f, -1.0f));
    Vertices.append(QVector3D(1.0f, 1.0f, -1.0f));
    Vertices.append(QVector3D(1.0f, -1.0f, -1.0f));
    Vertices.append(QVector3D(-1.0f, -1.0f, -1.0f));

    // Left face
    Vertices.append(QVector3D(-1.0f, -1.0f, -1.0f));
    Vertices.append(QVector3D(-1.0f, -1.0f, 1.0f));
    Vertices.append(QVector3D(-1.0f, 1.0f, 1.0f));
    Vertices.append(QVector3D(-1.0f, 1.0f, 1.0f));
    Vertices.append(QVector3D(-1.0f, 1.0f, -1.0f));
    Vertices.append(QVector3D(-1.0f, -1.0f, -1.0f));

    // Right face
    Vertices.append(QVector3D(1.0f, -1.0f, -1.0f));
    Vertices.append(QVector3D(1.0f, 1.0f, -1.0f));
    Vertices.append(QVector3D(1.0f, 1.0f, 1.0f));
    Vertices.append(QVector3D(1.0f, 1.0f, 1.0f));
    Vertices.append(QVector3D(1.0f, -1.0f, 1.0f));
    Vertices.append(QVector3D(1.0f, -1.0f, -1.0f));

    // Top face
    Vertices.append(QVector3D(-1.0f, 1.0f, -1.0f));
    Vertices.append(QVector3D(-1.0f, 1.0f, 1.0f));
    Vertices.append(QVector3D(1.0f, 1.0f, 1.0f));
    Vertices.append(QVector3D(1.0f, 1.0f, 1.0f));
    Vertices.append(QVector3D(1.0f, 1.0f, -1.0f));
    Vertices.append(QVector3D(-1.0f, 1.0f, -1.0f));

    // Bottom face
    Vertices.append(QVector3D(-1.0f, -1.0f, -1.0f));
    Vertices.append(QVector3D(1.0f, -1.0f, -1.0f));
    Vertices.append(QVector3D(1.0f, -1.0f, 1.0f));
    Vertices.append(QVector3D(1.0f, -1.0f, 1.0f));
    Vertices.append(QVector3D(-1.0f, -1.0f, 1.0f));
    Vertices.append(QVector3D(-1.0f, -1.0f, -1.0f));

    return Vertices;
}
