#include "AppData.h"

AppData::AppData()
{
}

AppData::~AppData()
{
}

void AppData::SetLatestDate()
{
	latestDate = std::numeric_limits<int32_t>::min();

	for (TimeLineEventListConstIter iter = eventList.begin(); iter != eventList.end(); iter++)
	{
		if (iter->endDate > latestDate)
		{
			latestDate = iter->endDate;
		}
	}
}
