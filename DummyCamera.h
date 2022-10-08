#ifndef DUMMYCAMERA_H
#define DUMMYCAMERA_H

#include "PerspectiveCamera.h"

#include <QObject>

class DummyCamera : public PerspectiveCamera
{
    Q_OBJECT
public:
    explicit DummyCamera(QObject *parent = nullptr);
    virtual ~DummyCamera();

    void mouseDoubleClicked(QMouseEvent *) override;
    void mousePressed(QMouseEvent *) override;
    void mouseReleased(QMouseEvent *) override;
    void mouseMoved(QMouseEvent *) override;
    void wheelMoved(QWheelEvent *) override;
    void keyPressed(QKeyEvent *) override;
    void keyReleased(QKeyEvent *) override;
    void update(float) override;
    void reset() override;
};

#endif // DUMMYCAMERA_H
