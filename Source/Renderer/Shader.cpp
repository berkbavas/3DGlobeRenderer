#include "Shader.h"

#include "Util/Misc.h"

#include <QDebug>
#include <QFile>

EarthRenderer::Shader::Shader(const QString& name)
    : mProgram(nullptr)
    , mName(name)
{
}

bool EarthRenderer::Shader::Initialize()
{
    initializeOpenGLFunctions();

    LOG_INFO("Shader::Initialize: '{}' is being initializing.", mName.toStdString());

    mProgram = QSharedPointer<QOpenGLShaderProgram>(new QOpenGLShaderProgram);

    for (const auto [shaderType, path] : mPaths)
    {
        const auto bytes = Misc::GetBytes(path);
        if (!mProgram->addShaderFromSourceCode(shaderType, bytes))
        {

            LOG_FATAL("Shader::Initialize: '{}' could not be loaded.", GetShaderTypeString(shaderType).toStdString());
            return false;
        }
    }

    if (!mProgram->link())
    {
        LOG_FATAL("Shader::Initialize: Could not link shader program.");
        return false;
    }

    if (!mProgram->bind())
    {
        LOG_FATAL("Shader::Initialize: Could not bind shader program.");
        return false;
    }

    LOG_INFO("Shader::Initialize: '{}' has been initialized.", mName.toStdString());

    return true;
}

bool EarthRenderer::Shader::Bind()
{
    return mProgram->bind();
}

void EarthRenderer::Shader::Release()
{
    mProgram->release();
}

void EarthRenderer::Shader::AddPath(QOpenGLShader::ShaderTypeBit type, const QString& path)
{
    mPaths.emplace(type, path);
}

QString EarthRenderer::Shader::GetName() const
{
    return mName;
}

QString EarthRenderer::Shader::GetShaderTypeString(QOpenGLShader::ShaderTypeBit type)
{
    switch (type)
    {
        case QOpenGLShader::Vertex:
            return "Vertex Shader";
        case QOpenGLShader::Fragment:
            return "Fragment Shader";
        case QOpenGLShader::Geometry:
            return "Geometry Shader";
        case QOpenGLShader::TessellationControl:
            return "Tessellation Control Shader";
        case QOpenGLShader::TessellationEvaluation:
            return "Tesselation Evaluation Shader";
        case QOpenGLShader::Compute:
            return "Compute Shader";
        default:
            return "Unknown Shader";
    }
}
