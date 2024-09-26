#include "Earth.h"

#include "Util/Logger.h"

#include <QFile>
#include <QImage>
#include <QImageReader>
#include <QOpenGLTexture>

EarthRenderer::Earth::Earth(QObject* parent)
    : Node(parent)
{
}

void EarthRenderer::Earth::Load(const QString& pathToModel, const QString& pathToTexture)
{
    LOG_DEBUG("Earth::Load: Loading Earth from '{}' and '{}'.", pathToModel.toStdString(), pathToTexture.toStdString());

    Parse(pathToModel);
    LoadTexture(pathToTexture);
    ConstructOpenGLStuff();

    LOG_DEBUG("Earth::Load: Earth has been successfully loaded.");
}

void EarthRenderer::Earth::Parse(const QString& pathToModel)
{
    LOG_DEBUG("Earth::Parse: Parsing 3D model...");

    QFile file(pathToModel);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        file.close();
        LOG_FATAL("Earth::Parse: '{:}' could not be opened.", file.fileName().toStdString());
        std::exit(EXIT_FAILURE);
    }

    QVector<QVector3D> position;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoords;

    QTextStream fileText(&file);

    while (!fileText.atEnd())
    {
        QString fileLine = fileText.readLine();
        if (fileLine.startsWith("vn "))
        {
            QStringList lineList = fileLine.split(" ");
            normals << QVector3D(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());
        }
        else if (fileLine.startsWith("vt "))
        {
            QStringList lineList = fileLine.split(" ");
            textureCoords << QVector2D(lineList[1].toFloat(), lineList[2].toFloat());
        }
        else if (fileLine.startsWith("v "))
        {
            QStringList lineList = fileLine.split(" ");
            position << QVector3D(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());
        }
        else if (fileLine.startsWith("f "))
        {
            QStringList lineList = fileLine.split(" ");
            for (int i = 1; i <= 3; i++)
            {
                QStringList arg = lineList[i].split("/");
                Vertex vertex;

                if (arg.size() == 2)
                {
                    if (arg[0].toInt() - 1 < position.size())
                        vertex.position = QVector3D(position[arg[0].toInt() - 1]);

                    if (arg[1].toInt() - 1 < normals.size())
                        vertex.normal = normals[arg[1].toInt() - 1];
                }
                else if (arg.size() == 3)
                {
                    if (arg[0].toInt() - 1 < position.size())
                        vertex.position = QVector3D(position[arg[0].toInt() - 1]);

                    if (arg[1].toInt() - 1 < textureCoords.size())
                        vertex.textureCoords = textureCoords[arg[1].toInt() - 1];

                    if (arg[2].toInt() - 1 < normals.size())
                        vertex.normal = normals[arg[2].toInt() - 1];
                }

                mVertices << vertex;
            }
        }
    }

    LOG_DEBUG("Earth::Parse: 3D model has been parsed.");
}

void EarthRenderer::Earth::LoadTexture(const QString& pathToTexture)
{
    LOG_DEBUG("Earth::LoadTexture: Loading textures...");

    QImageReader::setAllocationLimit(1024);

    QImage image = QImage(pathToTexture);

    if (image.isNull())
    {
        LOG_FATAL("Earth::LoadTexture: Image could not be loaded.");
        std::exit(EXIT_FAILURE);
    }

    mTexture = new QOpenGLTexture(image.mirrored());
    mTexture->setWrapMode(QOpenGLTexture::WrapMode::Repeat);
    mTexture->setMinMagFilters(QOpenGLTexture::Filter::LinearMipMapLinear, QOpenGLTexture::Filter::Linear);

    LOG_DEBUG("Earth::LoadTexture: Textures have been loaded.");
}

void EarthRenderer::Earth::ConstructOpenGLStuff()
{
    LOG_DEBUG("Earth::ConstructOpenGLStuff: Constructing OpenGL stuff...");

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

    LOG_DEBUG("Earth::ConstructOpenGLStuff: OpenGL stuff has been constructed.");
}

void EarthRenderer::Earth::BindTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture->textureId());
}

void EarthRenderer::Earth::Render()
{
    glBindVertexArray(mVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    glBindVertexArray(0);
}

void EarthRenderer::Earth::ReleaseTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
