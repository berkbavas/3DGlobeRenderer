#include "ShaderManager.h"
#include "Shader.h"

ShaderManager::ShaderManager(QObject *parent)
    : Manager(parent)
    , mActiveShader(ShaderType::None)
{}

Shader *ShaderManager::getShader(ShaderType shader)
{
    return mShaders.value(shader);
}

bool ShaderManager::init()
{
    // Model Shader
    {
        Shader *shader = new Shader(ShaderType::ModelShader);
        mShaders.insert(shader->type(), shader);

        shader->addPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Model.vert");
        shader->addPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Model.frag");

        QStringList uniforms;
        uniforms << "sun.color"
                 << "sun.direction"
                 << "sun.ambient"
                 << "sun.diffuse"
                 << "sun.specular"
                 << "node.transformation"
                 << "node.normalMatrix"
                 << "node.ambient"
                 << "node.diffuse"
                 << "node.specular"
                 << "node.shininess"
                 << "node.color"
                 << "cameraPosition"
                 << "VP";

        shader->addUniforms(uniforms);

        shader->addAttribute("position");
        shader->addAttribute("normal");
        shader->addAttribute("textureCoords");

        if (!shader->init())
            return false;
    }

    // Earth Shader
    {
        Shader *shader = new Shader(ShaderType::EarthShader);
        mShaders.insert(shader->type(), shader);

        shader->addPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Earth.vert");
        shader->addPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Earth.frag");

        QStringList uniforms;
        uniforms << "sun.color"
                 << "sun.direction"
                 << "sun.ambient"
                 << "sun.diffuse"
                 << "sun.specular"
                 << "earth.transformation"
                 << "earth.normalMatrix"
                 << "earth.ambient"
                 << "earth.diffuse"
                 << "earth.specular"
                 << "earth.shininess"
                 << "earth.color"
                 << "earth.height"
                 << "cameraPosition"
                 << "VP";

        shader->addUniforms(uniforms);

        shader->addAttribute("position");
        shader->addAttribute("normal");
        shader->addAttribute("textureCoords");

        if (!shader->init())
            return false;
    }

    // Earth Mouse Position Shader
    {
        Shader *shader = new Shader(ShaderType::EarthMousePositionShader);
        mShaders.insert(shader->type(), shader);

        shader->addPath(QOpenGLShader::Vertex, ":/Resources/Shaders/EarthMousePosition.vert");
        shader->addPath(QOpenGLShader::Fragment, ":/Resources/Shaders/EarthMousePosition.frag");

        shader->addUniform("MVP");

        shader->addAttribute("position");
        shader->addAttribute("normal");
        shader->addAttribute("textureCoords");

        if (!shader->init())
            return false;
    }

    return true;
}

bool ShaderManager::bind(ShaderType shader)
{
    mActiveShader = shader;
    return mShaders.value(mActiveShader)->bind();
}

void ShaderManager::release()
{
    mShaders.value(mActiveShader)->release();
}

void ShaderManager::setUniformValue(const QString &name, int value)
{
    mShaders.value(mActiveShader)->setUniformValue(name, value);
}

void ShaderManager::setUniformValue(const QString &name, unsigned int value)
{
    mShaders.value(mActiveShader)->setUniformValue(name, value);
}

void ShaderManager::setUniformValue(const QString &name, float value)
{
    mShaders.value(mActiveShader)->setUniformValue(name, value);
}

void ShaderManager::setUniformValue(const QString &name, const QVector3D &value)
{
    mShaders.value(mActiveShader)->setUniformValue(name, value);
}

void ShaderManager::setUniformValue(const QString &name, const QVector4D &value)
{
    mShaders.value(mActiveShader)->setUniformValue(name, value);
}

void ShaderManager::setUniformValue(const QString &name, const QMatrix4x4 &value)
{
    mShaders.value(mActiveShader)->setUniformValue(name, value);
}

void ShaderManager::setUniformValue(const QString &name, const QMatrix3x3 &value)
{
    mShaders.value(mActiveShader)->setUniformValue(name, value);
}

void ShaderManager::setUniformValueArray(const QString &name, const QVector<QVector3D> &values)
{
    mShaders.value(mActiveShader)->setUniformValueArray(name, values);
}

void ShaderManager::setSampler(const QString &name, unsigned int unit, unsigned int id, GLenum target)
{
    mShaders.value(mActiveShader)->setSampler(name, unit, id, target);
}

ShaderManager *ShaderManager::instance()
{
    static ShaderManager instance;

    return &instance;
}
