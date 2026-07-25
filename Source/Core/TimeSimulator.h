#pragma once

#include "Util/Macros.h"

#include <memory>

#include <QDateTime>
#include <QVector3D>

namespace GlobeRenderer
{
    class Sun;

    class TimeSimulator
    {
      public:
        explicit TimeSimulator(Sun* pSun);

        void Update(float Ifps);

        void CalculateSunPosition(const QDateTime& DateTime, float Latitude, float Longitude);
        void SetDateTime(const QDateTime& DateTime);
        QDateTime GetDateTime() const;
        void ResetToCurrentTime();

      private:
        Sun* mSun{ nullptr };

        // Animation enabled
        DEFINE_MEMBER(bool, Enabled, false);

        // Time speed multiplier (1.0 = real-time, 60.0 = 1 minute per second, etc.)
        DEFINE_MEMBER(float, TimeSpeed, 60.0f);

        // Current simulated time
        QDateTime mCurrentTime{ QDateTime::currentDateTime() };

        // Observer latitude/longitude for sun position calculation
        DEFINE_MEMBER(float, ObserverLatitude, 0.0f);
        DEFINE_MEMBER(float, ObserverLongitude, 0.0f);
    };

    using TimeSimulatorPtr = std::unique_ptr<TimeSimulator>;
}
