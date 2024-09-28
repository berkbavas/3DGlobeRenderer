#include "Util.h"

#include "Util/Logger.h"

#include <QFile>
#include <QImage>
#include <QTextStream>
#include <QVector2D>
#include <QVector3D>

QByteArray GlobeRenderer::Util::GetBytes(const QString& path)
{
    QFile file(path);

    if (file.open(QFile::ReadOnly))
    {
        return file.readAll();
    }
    else
    {
        LOG_WARN("Util::GetBytes: '{}' could not be opened", path.toStdString());
        return QByteArray();
    }
}

QVector<GlobeRenderer::Vertex> GlobeRenderer::Util::Parse(const QString& path)
{
    LOG_DEBUG("Util::Parse: Parsing 3D model...");

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        file.close();
        LOG_FATAL("Util::Parse: '{:}' could not be opened.", file.fileName().toStdString());
        return QVector<GlobeRenderer::Vertex>();
    }

    QVector<Vertex> vertices;
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

                vertices << vertex;
            }
        }
    }

    LOG_DEBUG("Util::Parse: 3D model has been parsed.");

    return vertices;
}
