#pragma once

#include "Core/FailureBehaviour.h"
#include "Util/Logger.h"

#include <QObject>
#include <QOpenGLShader>
#include <map>

namespace GlobeRenderer
{
    class Shader
    {
      public:
        Shader(const QString& name);

        void Initialize();
        bool Bind();
        void Release();

        void AddPath(QOpenGLShader::ShaderTypeBit type, const QString& path);

        QString GetName() const;

        static QString GetShaderTypeString(QOpenGLShader::ShaderTypeBit type);

        template<typename T>
        void SetUniformValue(const QString& name, T value)
        {
            const auto location = mProgram->uniformLocation(name);

            if (0 <= location)
            {
                mProgram->setUniformValue(location, value);
            }
            else
            {
                LOG_FATAL("Shader::SetUniformValue: Uniform location '{}' could not be found.", name.toStdString());
                FailureBehaviour::Failure(FailureType::COULD_NOT_FIND_UNIFORM_LOCATION);
            }
        }

        template<typename T>
        void SetUniformValueArray(const QString& name, const QVector<T>& values)
        {
            const auto location = mProgram->uniformLocation(name);

            if (0 <= location)
            {
                mProgram->setUniformValueArray(location, values.constData(), values.size());
            }
            else
            {
                LOG_FATAL("Shader::SetUniformValue: Uniform location '{}' could not be found.", name.toStdString());
                FailureBehaviour::Failure(FailureType::COULD_NOT_FIND_UNIFORM_LOCATION);
            }
        }

      private:
        QSharedPointer<QOpenGLShaderProgram> mProgram;
        std::map<QOpenGLShader::ShaderTypeBit, QString> mPaths;

        QString mName;
    };
}
