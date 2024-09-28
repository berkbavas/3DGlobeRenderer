#include "Core/Controller.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    EarthRenderer::Controller* controller = new EarthRenderer::Controller;

    controller->Run();

    return app.exec();
}
