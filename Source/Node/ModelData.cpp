#include "ModelData.h"

#include "Util/Logger.h"
#include "Util/Util.h"

GlobeRenderer::ModelData::ModelData(const QString& path)
{
    LOG_DEBUG("ModelData::ModelData: Creating model from '{}'", path.toStdString());

    mVertices = Util::Parse(path);

    if (mVertices.isEmpty())
    {
        LOG_FATAL("ModelData::ModelData: Cannot proceed without model. Exiting...");
        std::exit(EXIT_FAILURE);
    }

    LOG_DEBUG("ModelData::ModelData: Constructing OpenGL stuff...");

    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.constData(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, textureCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    LOG_DEBUG("ModelData::ModelData: OpenGL stuff has been constructed.");
}

void GlobeRenderer::ModelData::Render()
{
    glBindVertexArray(mVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
}