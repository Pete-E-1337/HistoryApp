#ifndef WAVEBLADEDRIVER_H
#define WAVEBLADEDRIVER_H

#include "Settings.h"

class WavebladeDriver
{
public:
   enum GearType
   {
      GearType_Reverse,
      GearType_Neutral,
      GearType_Forward
   };

   enum ThrottleType
   {
      ThrottleType_Drift,
      ThrottleType_Crawl,
      ThrottleType_Cruise,
      ThrottleType_Sprint,
      ThrottleType_ElectricOnly
   };

public:
   WavebladeDriver(Settings* settings);
   virtual ~WavebladeDriver();

   void SetGear(GearType gear);
   void SetThrottle(ThrottleType throttle);

private:
   Settings*      m_settings     = nullptr;
   GearType       m_gear         = GearType_Neutral;
   ThrottleType   m_throttle     = ThrottleType_Drift;
};

#endif // WAVEBLADEDRIVER_H

