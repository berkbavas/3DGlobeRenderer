#include "TimeSimulator.h"

#include "Node/Sun.h"

#include <QtMath>

GlobeRenderer::TimeSimulator::TimeSimulator(Sun* pSun)
    : mSun(pSun)
{}

void GlobeRenderer::TimeSimulator::Update(float Ifps)
{
    if (!mEnabled)
    {
        return;
    }

    // Advance time
    qint64 MillisecondsToAdd = static_cast<qint64>(mTimeSpeed * Ifps * 1000.0f);
    mCurrentTime = mCurrentTime.addMSecs(MillisecondsToAdd);

    // Calculate sun position based on time
    // Simplified solar position calculation
    QDate Date = mCurrentTime.date();
    QTime Time = mCurrentTime.time();

    // Day of year (1-365)
    int DayOfYear = Date.dayOfYear();

    // Time in hours (0-24)
    float Hours = Time.hour() + Time.minute() / 60.0f + Time.second() / 3600.0f;

    // Solar declination (simplified)
    float Declination = 23.45f * qSin(qDegreesToRadians(360.0f / 365.0f * (DayOfYear - 81)));

    // Hour angle (15 degrees per hour from solar noon)
    float HourAngle = 15.0f * (Hours - 12.0f);

    // Convert to theta/phi for sun direction
    float Theta = HourAngle;
    float Phi = Declination;

    mSun->SetDirectionFromThetaPhi(Theta, Phi);
}

void GlobeRenderer::TimeSimulator::CalculateSunPosition(const QDateTime& DateTime, float Latitude, float Longitude)
{
    mCurrentTime = DateTime;
    mObserverLatitude = Latitude;
    mObserverLongitude = Longitude;
}

void GlobeRenderer::TimeSimulator::SetDateTime(const QDateTime& DateTime)
{
    mCurrentTime = DateTime;
}

QDateTime GlobeRenderer::TimeSimulator::GetDateTime() const
{
    return mCurrentTime;
}

void GlobeRenderer::TimeSimulator::ResetToCurrentTime()
{
    mCurrentTime = QDateTime::currentDateTime();
}
