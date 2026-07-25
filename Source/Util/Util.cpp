#include "Util.h"

#include "Util/Logger.h"

#include <QFile>
#include <QUuid>

QByteArray GlobeRenderer::Util::GetBytes(const QString& Path)
{
    QFile File(Path);

    if (File.open(QFile::ReadOnly))
    {
        return File.readAll();
    }
    else
    {
        LOG_WARN("Util::GetBytes: '{}' could not be opened.", Path.toStdString());
        return QByteArray();
    }
}

QVector3D GlobeRenderer::Util::GenerateRandomVector(float X, float Y, float Z)
{
    return QVector3D(GenerateRandom(X), GenerateRandom(Y), GenerateRandom(Z));
}

float GlobeRenderer::Util::GenerateRandom(float Bound)
{
    return mGenerator.bounded(Bound);
}

bool GlobeRenderer::Util::WriteTextToFile(const QString& Path, const QByteArray& Content)
{
    QFile File(Path);

    if (File.open(QIODevice::WriteOnly))
    {
        QTextStream Stream(&File);
        Stream.setEncoding(QStringConverter::Utf8);
        Stream << Content;
        Stream.flush();
        File.close();
        return true;
    }
    else
    {
        LOG_FATAL("Util::WriteTextToFile: Could not write to file: {}", Path.toStdString());
        return false;
    }
}

bool GlobeRenderer::Util::WriteDataToFile(const QString& Path, const QByteArray& Content)
{
    QFile File(Path);
    if (File.open(QIODevice::WriteOnly))
    {
        QDataStream Stream(&File);
        Stream << Content;
        File.close();
        return true;
    }
    else
    {
        LOG_FATAL("Util::WriteDataToFile: Could not write to file: {}", Path.toStdString());
        return false;
    }
}

QByteArray GlobeRenderer::Util::ReadDataFromFile(const QString& Path)
{
    QFile File(Path);
    if (File.open(QIODevice::ReadOnly))
    {
        QByteArray Array;
        QDataStream Stream(&File);
        Stream >> Array;
        File.close();
        return Array;
    }
    else
    {
        LOG_FATAL("Util::ReadDataFromFile: Could not read from file: {}", Path.toStdString());
        return QByteArray();
    }
}

QJsonDocument GlobeRenderer::Util::ReadJson(const QString& Path)
{
    QJsonDocument Document;

    QFile File(Path);
    if (!File.open(QIODevice::ReadOnly))
    {
        LOG_FATAL("Util::ReadJson: Error occured while reading the file: {}", Path.toStdString());
        return QJsonDocument();
    }

    Document = QJsonDocument::fromJson(File.readAll());
    File.close();

    return Document;
}

const char* GlobeRenderer::Util::GetGlErrorString(GLenum Error)
{
    switch (Error)
    {
    case GL_NO_ERROR:
        return "No error";
    case GL_INVALID_ENUM:
        return "Invalid enum";
    case GL_INVALID_VALUE:
        return "Invalid value";
    case GL_INVALID_OPERATION:
        return "Invalid operation";
    case GL_STACK_OVERFLOW:
        return "Stack overflow";
    case GL_STACK_UNDERFLOW:
        return "Stack underflow";
    case GL_OUT_OF_MEMORY:
        return "Out of memory";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "Invalid framebuffer operation";
    default:
        return "Unknown error";
    }
}

QRandomGenerator GlobeRenderer::Util::mGenerator = QRandomGenerator::securelySeeded();
