#pragma once

#include <QByteArray>
#include <QString>

namespace EarthRenderer
{
    class Misc
    {
      public:
        Misc() = delete;

        static QByteArray GetBytes(const QString& path);
    };
}
