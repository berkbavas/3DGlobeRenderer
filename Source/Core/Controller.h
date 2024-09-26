#pragma once

#include <QMouseEvent>
#include <QObject>

namespace EarthRenderer
{
    class Renderer;
    class Window;

    class Controller : public QObject
    {
        Q_OBJECT
      public:
        explicit Controller(QObject* parent = nullptr);
        ~Controller();

        void Run();

      public slots:
        void Initialize();
        void Resize(int w, int h);
        void Update(float ifps);
        void Render(float ifps);
        void KeyPressed(QKeyEvent*);
        void KeyReleased(QKeyEvent*);
        void MousePressed(QMouseEvent*);
        void MouseReleased(QMouseEvent*);
        void MouseMoved(QMouseEvent*);
        void WheelMoved(QWheelEvent*);

      private:
        Window* mWindow;
        Renderer* mRenderer;
    };
}
