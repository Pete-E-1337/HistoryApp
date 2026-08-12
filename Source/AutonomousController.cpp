#include "AutonomousController.h"

AutonomousController::AutonomousController(Settings* settings, WavebladeDriver* wavebladeDriver) :
   m_settings(settings),
   m_wavebladeDriver(wavebladeDriver)
{
}

AutonomousController::~AutonomousController()
{
}

void AutonomousController::SetEnabled(bool enabled)
{
   m_enabled = enabled;
}
