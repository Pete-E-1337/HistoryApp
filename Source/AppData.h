#ifndef APPDATA_H
#define APPDATA_H

//#include "Settings.h"
#include <string>
#include <vector>

typedef struct TimelineEventData
{
	double			startDate;
	double			endDate;
	std::string		name;
} TimelineEventData;

typedef std::vector<TimelineEventData>		TimeLineEventList;
typedef TimeLineEventList::iterator			TimeLineEventListIter;
typedef TimeLineEventList::const_iterator	TimeLineEventListConstIter;

class AppData
{
public:
   AppData();
   virtual ~AppData();

	//Settings*					settings					= nullptr;
	//WavebladeDriver*        wavebladeDriver		= nullptr;
	//AutonomousController*	autonomousController	= nullptr;
	//ManualController*			manualController		= nullptr;
	//ClientComms*				clientComms				= nullptr;
	//DebugData*					debugData				= nullptr;
	//bool							freezeSimulation		= false;
	//bool							reset						= false;
	TimeLineEventList		eventList;
	double					newestDate;
	
};

#endif // APPDATA_H
