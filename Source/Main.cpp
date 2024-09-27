#include "Core/Controller.h"

#include <QApplication>
#include <QImageReader>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QImageReader::setAllocationLimit(1024);
    EarthRenderer::Controller* controller = new EarthRenderer::Controller;

    controller->Run();

    return app.exec();
}
