#pragma once

#include "Node/Node.h"

#include <QOpenGLExtraFunctions>

namespace GlobeRenderer
{
    class Renderable : public Node, protected QOpenGLExtraFunctions
    {
        virtual void Construct() = 0;
        virtual void Render() = 0;
        virtual void Destroy() = 0;
    };
}