#include "Misc.h"

#include "Util/Logger.h"

#include <QFile>

QByteArray EarthRenderer::Misc::GetBytes(const QString& path)
{
    QFile file(path);

    if (file.open(QFile::ReadOnly))
    {
        return file.readAll();
    }
    else
    {
        LOG_WARN("Misc::GetBytes: '{}' could not be opened", path.toStdString());
        return QByteArray();
    }
}
