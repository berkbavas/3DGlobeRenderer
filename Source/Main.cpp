#include "Core/Controller.h"

#include <QApplication>
#include <QImageReader>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QImageReader::setAllocationLimit(0);

    GlobeRenderer::Controller* controller = new GlobeRenderer::Controller;

    controller->Run();

    return app.exec();
}
