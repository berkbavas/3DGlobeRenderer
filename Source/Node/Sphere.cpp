#include "Sphere.h"

#include "Util/Logger.h"

GlobeRenderer::Sphere::Sphere(float Radius, int StackCount, int SectorCount)
{
    LOG_DEBUG("Sphere::Sphere: Initializing Sphere...");

    CreateGeometry(Radius, StackCount, SectorCount);

    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mFaces.size() * sizeof(Sphere::TriangleFace), mFaces.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Sphere::Vertex), mVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Sphere::Vertex), (void *) offsetof(Sphere::Vertex, Position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Sphere::Vertex), (void *) offsetof(Sphere::Vertex, Normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Sphere::Vertex), (void *) offsetof(Sphere::Vertex, TextureCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    mVertices.clear();
    mFaces.clear();

    LOG_DEBUG("Sphere::Sphere: Sphere has been initialized.");
}

GlobeRenderer::Sphere::~Sphere()
{
    LOG_DEBUG("Sphere::~Sphere: Deleting OpenGL stuff...");

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

    if (mEBO)
    {
        glDeleteBuffers(1, &mEBO);
        mEBO = 0;
    }

    LOG_DEBUG("Sphere::~Sphere: OpenGL stuff has been deleted.");
}

void GlobeRenderer::Sphere::Render()
{
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, 3 * mNumberOfFaces, GL_UNSIGNED_INT, 0); // 3 vertex per triangle
    glBindVertexArray(0);
}

void GlobeRenderer::Sphere::CreateGeometry(float Radius, int StackCount, int SectorCount)
{
    Vertex TempVertex;

    // Generate vertices
    for (int i = 0; i <= StackCount; ++i)
    {
        float StackAngle = M_PI / 2 - i * M_PI / StackCount; // from pi/2 to -pi/2
        float xy = Radius * cosf(StackAngle);                // r * cos(u)
        float z = Radius * sinf(StackAngle);                 // r * sin(u)

        for (int j = 0; j <= SectorCount; ++j)
        {
            float SectorAngle = j * 2 * M_PI / SectorCount; // from 0 to 2pi

            TempVertex.Position.setX(xy * cosf(SectorAngle)); // x = r * cos(u) * cos(v)
            TempVertex.Position.setY(xy * sinf(SectorAngle)); // y = r * cos(u) * sin(v)
            TempVertex.Position.setZ(z);                      // z = r * sin(u)

            TempVertex.Normal = TempVertex.Position.normalized();

            TempVertex.TextureCoords.setX(static_cast<float>(j) / SectorCount);
            TempVertex.TextureCoords.setY(static_cast<float>(i) / StackCount);

            mVertices.push_back(TempVertex);
        }
    }

    // Generate indices
    for (int i = 0; i < StackCount; ++i)
    {
        for (int j = 0; j < SectorCount; ++j)
        {
            const unsigned int First = (i * (SectorCount + 1)) + j;
            const unsigned int Second = First + SectorCount + 1;

            if (i != 0)
            {
                mFaces << Sphere::TriangleFace{ First, Second, First + 1 };
            }

            if (i != (StackCount - 1))
            {
                mFaces << Sphere::TriangleFace{ First + 1, Second, Second + 1 };
            }
        }
    }

    mNumberOfFaces = mFaces.size();
}
