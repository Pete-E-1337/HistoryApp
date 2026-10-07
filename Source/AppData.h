#ifndef APPDATA_H
#define APPDATA_H

//#define USE_THUMBNAILS
//#define USE_THUMBNAIL_RESIZE

//#include "Settings.h"
#include <string>
#include <vector>
#ifdef USE_THUMBNAILS
#include <wx/bitmap.h> // Required header for wxBitmap
#endif

static const double l_latLongUninitialized	= 999.0;

typedef struct TimelineEventData
{
	std::string		name;
	double			startDate;
	double			endDate;
	double			latitude;
	double			longitude;
	std::string		imageFilename;
	int				id;
#ifdef USE_THUMBNAILS
	wxBitmap			thumbnailBitmap;
	wxImage			thumbnailImage;
#endif
} TimelineEventData;

typedef std::vector<TimelineEventData>		TimeLineEventList;
typedef TimeLineEventList::iterator			TimeLineEventListIter;
typedef TimeLineEventList::const_iterator	TimeLineEventListConstIter;

typedef struct BattleData
{
	std::string		name;
	double			startDate;
	double			endDate;
	double			latitude;
	double			longitude;
	std::string		startDateString;
	std::string		endDateString;
	std::string		personnel;
	std::string		combatants;
	std::string		winner;
	std::string		victory_size;
	int				mapImageXCoord;
	int				mapImageYCoord;
	int				id;
} BattleData;

typedef std::vector<BattleData>		BattleList;
typedef BattleList::iterator			BattleListIter;
typedef BattleList::const_iterator	BattletListConstIter;

class AppData
{
public:
   AppData();
   virtual ~AppData();

	void SetLatestDate();

	//Settings*					settings					= nullptr;
	TimeLineEventList			eventList;
	TimeLineEventList			imageList;
	BattleList					battleList;
	double						latestDate;
	bool							rendering				= true;
	bool							updateDateRangeText	= true;
	bool							updateDateScrollbar	= false;
	bool							updateSelection		= true;
};

#endif // APPDATA_H
