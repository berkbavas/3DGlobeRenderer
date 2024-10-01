#pragma once

#include <QByteArray>
#include <QString>

namespace GlobeRenderer
{
    class Util
    {
      public:
        Util() = delete;

        static QByteArray GetBytes(const QString& path);
    };
}
