#pragma once

#include "Node/Camera/Camera.h"

namespace GlobeRenderer
{
    class PerspectiveCamera : public Camera
    {
      protected:
        PerspectiveCamera() = default;

      public:
        virtual const QMatrix4x4& GetProjectionMatrix() override;
        float GetHorizontalFov() const;

      private:
        DEFINE_MEMBER(float, VerticalFov, 60.0f);

        // We do not want to create QMatrix4x4 object for each call of GetProjectionMatrix()
        QMatrix4x4 mProjectionMatrix;
    };

    using PerspectiveCameraPtr = QSharedPointer<PerspectiveCamera>;
}
