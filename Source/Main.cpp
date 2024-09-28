#include "Core/Controller.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    GlobeRenderer::Controller* controller = new GlobeRenderer::Controller;

    controller->Run();

    return app.exec();
}
