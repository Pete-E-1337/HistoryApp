#include "WavebladeDriver.h"

WavebladeDriver::WavebladeDriver(Settings* settings) :
   m_settings(settings)
{
}

WavebladeDriver::~WavebladeDriver()
{
}

void WavebladeDriver::SetGear(GearType gear)
{
   m_gear = gear;
}

void WavebladeDriver::SetThrottle(ThrottleType throttle)
{
   m_throttle = throttle;
}
