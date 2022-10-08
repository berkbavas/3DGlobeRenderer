#include "DummyCamera.h"

DummyCamera::DummyCamera(QObject *parent)
    : PerspectiveCamera(parent)
{
    mNodeType = Node::NodeType::DummyCamera;
}

DummyCamera::~DummyCamera() {}

void DummyCamera::mouseDoubleClicked(QMouseEvent *) {}

void DummyCamera::mousePressed(QMouseEvent *) {}

void DummyCamera::mouseReleased(QMouseEvent *) {}

void DummyCamera::mouseMoved(QMouseEvent *) {}

void DummyCamera::wheelMoved(QWheelEvent *) {}

void DummyCamera::keyPressed(QKeyEvent *) {}

void DummyCamera::keyReleased(QKeyEvent *) {}

void DummyCamera::update(float) {}

void DummyCamera::reset() {}
