#ifndef AUTONOMOUSCONTROLLER_H
#define AUTONOMOUSCONTROLLER_H

#include "Settings.h"
#include "WavebladeDriver.h"

class AutonomousController
{
public:
   AutonomousController(Settings* settings, WavebladeDriver* wavebladeDriver);
   virtual ~AutonomousController();

   void SetEnabled(bool enabled);

private:
   Settings*         m_settings           = nullptr;
   WavebladeDriver*  m_wavebladeDriver    = nullptr;
   bool              m_enabled            = false;
};

#endif // AUTONOMOUSCONTROLLER_H

