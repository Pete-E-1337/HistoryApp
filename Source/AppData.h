#ifndef APPDATA_H
#define APPDATA_H

//#include "Settings.h"
#include <string>
#include <vector>

typedef struct TimelineEventData
{
	std::string		name;
	double			startDate;
	double			endDate;
	double			latitude;
	double			longitude;
	std::string		imageFilename;
	int				id;
} TimelineEventData;

typedef std::vector<TimelineEventData>		TimeLineEventList;
typedef TimeLineEventList::iterator			TimeLineEventListIter;
typedef TimeLineEventList::const_iterator	TimeLineEventListConstIter;

class AppData
{
public:
   AppData();
   virtual ~AppData();

	void SetLatestDate();

	//Settings*					settings					= nullptr;
	TimeLineEventList			eventList;
	TimeLineEventList			imageList;
	double						latestDate;
	bool							rendering				= true;
};

#endif // APPDATA_H
