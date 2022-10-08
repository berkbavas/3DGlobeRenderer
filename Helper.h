#ifndef HELPER_H
#define HELPER_H

#include "ModelData.h"

#include <QMatrix4x4>
#include <QQuaternion>
#include <QString>

class Helper
{
private:
    Helper();

public:
    static QByteArray getBytes(QString path);
    static QQuaternion rotateX(float angleRadians);
    static QQuaternion rotateY(float angleRadians);
    static QQuaternion rotateZ(float angleRadians);
    static QQuaternion invert(const QQuaternion &rotation);
    static float angleBetween(const QVector3D &v1, const QVector3D &v2, const QVector3D &left);
    static float angleBetween(const QVector3D &v1, const QVector3D &v2);
    static void getEulerDegrees(const QQuaternion &rotation, float &yaw, float &pitch, float &roll);
    static QQuaternion constructFromEulerDegrees(float yaw, float pitch, float roll);
    static QVector3D constructFromLatLon(float lat, float lon);
    static ModelData *load(const QString &path);
};

#endif // HELPER_H
