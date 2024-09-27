#include "Earth.h"

#include "Util/Logger.h"
#include "Util/Util.h"

EarthRenderer::Earth::Earth(QObject* parent)
    : Node(parent)
{
}

void EarthRenderer::Earth::Initialize(const QString& path)
{
    LOG_DEBUG("Earth::Initialize: Constructing OpenGL stuff...");

    mVertices = Util::Parse(path);

    if (mVertices.isEmpty())
    {
        LOG_FATAL("Renderer::Initialize: Cannot proceed without model. Exiting...");
        std::exit(EXIT_FAILURE);
    }

    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.constData(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, textureCoords));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    LOG_DEBUG("Earth::Initialize: OpenGL stuff has been constructed.");
}

void EarthRenderer::Earth::Render()
{
    glBindVertexArray(mVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
}

void EarthRenderer::Earth::BindTextures()
{
    for (const auto& [unit, texture] : mTextures)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture->textureId());
    }
}
void EarthRenderer::Earth::ReleaseTextures()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void EarthRenderer::Earth::AddTexture(GLuint unit, const QString& path)
{
    if (const auto texture = Util::LoadTexture(path))
    {
        mTextures.emplace(unit, texture);
    }
    else
    {
        LOG_FATAL("Earth::AddTexture: Texture could not be loaded. Exiting...");
        std::exit(EXIT_FAILURE);
    }
}
