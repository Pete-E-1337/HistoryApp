#ifndef PENGUIN_MAPS_H
#define PENGUIN_MAPS_H

// We want to be able to answer "give me the pixel location on the map that corresponds to this given lat/lon"
// The reference map image is "Index Map- marked.JPG"

// Using Decimal Degrees (DD) (Uses numbers with positive or negative signs instead of letters. North and East are positive.)

typedef struct MapPixelData
{
	int			x;
	int			y;
} MapPixelData;

static const int l_array_rows = 10;
static const int l_array_cols = 11;

static const int l_map_width	= 2570;
static const int l_map_height	= 2088;

// grid of 10 latitudes by 11 longitudes, Row x Column
static const MapPixelData l_map_pixel_data[l_array_rows][l_array_cols] =
{
	//    -20          -10            0           10            20           30             40            50            60            70          80
	{ {195,   44}, {371,   94}, {549,  130}, {736,  142}, { 925,  131}, {1102,   95}, {1286,   34}, {   0,    0}, {   0,    0}, {   0,   0}, {   0,  0} },	// Row 0, 65 degree latitude
	{ { 92,  248}, {294,  309}, {511,  351}, {731,  369}, { 956,  357}, {1167,  312}, {1379,  236}, {1575,  124}, {   0,    0}, {   0,   0}, {   0,  0} },	// Row 1, 60 degree latitude
	{ {  3,  453}, {228,  527}, {478,  572}, {733,  587}, { 985,  567}, {1227,  516}, {1470,  434}, {1693,  309}, {1894,  155}, {   0,   0}, {   0,  0} },	// Row 2, 55 degree latitude
	{ {  0,    0}, {172,  734}, {447,  792}, {736,  808}, {1016,  787}, {1287,  729}, {1555,  634}, {1804,  496}, {2024,  327}, {2222, 128}, {   0,  0} },	// Row 3, 50 degree latitude
	{ {  0,    0}, {123,  948}, {419, 1013}, {739, 1033}, {1046, 1014}, {1348,  952}, {1641,  841}, {1915,  697}, {2162,  508}, {2377, 287}, {2552, 33} },	// Row 4, 45 degree latitude
	{ {  0,    0}, { 74, 1164}, {400, 1234}, {743, 1255}, {1076, 1234}, {1405, 1160}, {1724, 1046}, {2020,  890}, {2293,  692}, {2535, 454}, {   0,  0} },	// Row 5, 40 degree latitude
	{ {  0,    0}, { 29, 1373}, {382, 1451}, {749, 1479}, {1107, 1455}, {1466, 1376}, {1806, 1252}, {2126, 1089}, {2420,  877}, {   0,   0}, {   0,  0} },	// Row 6, 35 degree latitude
	{ {  0,    0}, {  0,    0}, {374, 1677}, {756, 1693}, {1136, 1664}, {1523, 1583}, {1884, 1461}, {2224, 1285}, {2536, 1058}, {   0,   0}, {   0,  0} },	// Row 7, 30 degree latitude
	{ {  0,    0}, {  0,    0}, {372, 1894}, {762, 1908}, {1164, 1877}, {1571, 1794}, {1955, 1671}, {2312, 1477}, {   0,    0}, {   0,   0}, {   0,  0} },	// Row 8, 25 degree latitude
	{ {  0,    0}, {  0,    0}, {  0,    0}, {  0,    0}, {   0,    0}, {1613, 2027}, {2023, 1884}, {2404, 1695}, {   0,    0}, {   0,   0}, {   0,  0} }		// Row 9, 20 degree latitude
};

static const int l_map_row_lat_data[l_array_rows] = { 65, 60, 55, 50, 45, 40, 35, 30, 25, 20 };
static const int l_map_col_lon_data[l_array_cols] = { -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80 };

//typedef std::vector<TimelineEventData>		TimeLineEventList;
//typedef TimeLineEventList::iterator			TimeLineEventListIter;
//typedef TimeLineEventList::const_iterator	TimeLineEventListConstIter;

class PenguinMaps
{
public:
   PenguinMaps();
   virtual ~PenguinMaps();

	static int GetMapWidth() { return 2570; }
	static int GetMapHeight() { return 2088; }
	static void GetPixelLocation(double lat, double lon, MapPixelData* pixel_data);

private:
	static int GetArrayRowIndexForLat(double lat);	// For a given lat, give the row index of the lower bounding latitude
	static int GetArrayColIndexForLon(double lon);	// For a given lon, give the col index of the lower bounding longitude
};

#endif // PENGUIN_MAPS_H
