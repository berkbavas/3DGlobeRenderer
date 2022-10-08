#ifndef FREECAMERA_H
#define FREECAMERA_H

#include "PerspectiveCamera.h"

#include <QObject>

class FreeCamera : public PerspectiveCamera
{
    Q_OBJECT
public:
    explicit FreeCamera(QObject *parent = nullptr);
    virtual ~FreeCamera();

    void mouseDoubleClicked(QMouseEvent *) override;
    void mousePressed(QMouseEvent *event) override;
    void mouseReleased(QMouseEvent *event) override;
    void mouseMoved(QMouseEvent *event) override;
    void keyPressed(QKeyEvent *event) override;
    void keyReleased(QKeyEvent *event) override;
    void wheelMoved(QWheelEvent *) override;
    void update(float ifps) override;
    void reset() override;
    void drawGUI() override;

private:
    struct Mouse {
        bool pressed;
        float x;
        float y;
        float dx;
        float dy;
    };

    struct Speed {
        float movement;
        float angular;
        float movementMultiplier;
        float angularMultiplier;
    };

    QMap<Qt::Key, bool> mPressedKeys;

    Speed mSpeed;
    Mouse mMouse;

    bool mUpdateRotation;
    bool mUpdatePosition;

    float mTimeElapsed;

    static const QMap<Qt::Key, QVector3D> KEY_BINDINGS;
};

#endif // FREECAMERA_H
