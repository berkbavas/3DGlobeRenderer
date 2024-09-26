#pragma once

#include "Util/Logger.h"

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QScopedPointer>
#include <QSharedPointer>
#include <map>

namespace EarthRenderer
{
    class Shader : protected QOpenGLFunctions
    {
      public:
        Shader(const QString& name);

        bool Initialize();
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
                LOG_FATAL("Shader::SetUniformValue: Location '{}' is not found.", name.toStdString());
                std::exit(EXIT_FAILURE);
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
                LOG_FATAL("Shader::SetUniformValue: Location '{}' is not found.", name.toStdString());
                std::exit(EXIT_FAILURE);
            }
        }

      private:
        QSharedPointer<QOpenGLShaderProgram> mProgram;
        std::map<QOpenGLShader::ShaderTypeBit, QString> mPaths;

        QString mName;
    };
}
