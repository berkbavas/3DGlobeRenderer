#pragma once

#include "Core/Constants.h"
#include "Util/Macros.h"

#include <QMouseEvent>
#include <QOpenGLExtraFunctions>

namespace GlobeRenderer
{
    class Window;
    class Renderer;
    class Camera;
    class Globe;

    class Controller : public QObject, protected QOpenGLExtraFunctions
    {
        DISABLE_COPY(Controller);

        Q_OBJECT
      public:
        explicit Controller(QObject* parent = nullptr);
        ~Controller();

        void Run();

      public slots:
        // Core Events
        void Initialize();
        void Resize(int w, int h);
        void Render(float ifps);

        // Input Events
        void OnKeyPressed(QKeyEvent*);
        void OnKeyReleased(QKeyEvent*);
        void OnMousePressed(QMouseEvent*);
        void OnMouseReleased(QMouseEvent*);
        void OnMouseMoved(QMouseEvent*);
        void OnWheelMoved(QWheelEvent*);

      private:
        float mDevicePixelRatio{ 1.0f };
        float mWidth{ INITIAL_WIDTH };
        float mHeight{ INITIAL_HEIGHT };

        Window* mWindow;
        Renderer* mRenderer;
        Camera* mCamera;
        Globe* mGlobe;
    };
}
