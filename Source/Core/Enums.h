#pragma once

namespace GlobeRenderer
{
    enum class ViewDirection
    {
        Front,
        Back,
        Up,
        Down,
        Left,
        Right
    };

    enum FramebufferType
    {
        Multisample,
        Singlesample
    };

}