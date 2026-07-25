#include "Core/Renderer.h"

#include <QApplication>

int main(int Argc, char* Argv[])
{
    QApplication Application(Argc, Argv);

    GlobeRenderer::Renderer Renderer;
    Renderer.Run();

    return Application.exec();
}
