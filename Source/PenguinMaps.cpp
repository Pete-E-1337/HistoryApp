#include "PenguinMaps.h"

#include <SVSLibrary/Compiler/Warnings.h>
#include <SVSLibrary/Spatial/Vector2D.h>

SVS_WARNING_DISABLE(4100) // unreferenced formal parameter
SVS_WARNING_DISABLE(4189) // local variable is initialized but not referenced

PenguinMaps::PenguinMaps()
{
}

PenguinMaps::~PenguinMaps()
{
}

int PenguinMaps::GetArrayRowIndexForLat(double lat)
{
	int index = -1;

	if (lat < 20.0)
		index = -1;
	else if (lat < 25.0)
		index = 9;
	else if (lat < 30.0)
		index = 8;
	else if (lat < 35.0)
		index = 7;
	else if (lat < 40.0)
		index = 6;
	else if (lat < 45.0)
		index = 5;
	else if (lat < 50.0)
		index = 4;
	else if (lat < 55.0)
		index = 3;
	else if (lat < 60.0)
		index = 2;
	else if (lat < 65.0)
		index = 1;
	else if (lat < 70.0)
		index = 0;
	else
		index = -1;

	return index;
}

int PenguinMaps::GetArrayColIndexForLon(double lon)
{
	int index = -1;

	if (lon < -20.0)
		index = -1;
	else if (lon < -10.0)
		index = 0;
	else if (lon < 0.0)
		index = 1;
	else if (lon < 10.0)
		index = 2;
	else if (lon < 20.0)
		index = 3;
	else if (lon < 30.0)
		index = 4;
	else if (lon < 40.0)
		index = 5;
	else if (lon < 50.0)
		index = 6;
	else if (lon < 60.0)
		index = 7;
	else if (lon < 70.0)
		index = 8;
	else if (lon < 80.0)
		index = 9;
	else
		index = -1;

	return index;
}

void PenguinMaps::GetPixelLocation(double lat, double lon, MapPixelData* pixel_data)
{
	int row = GetArrayRowIndexForLat(lat);
	int col = GetArrayColIndexForLon(lon);

	if ((row == -1) || (col == -1))
		return;

	SVS::Vector2Dd p1, p2, p3, p4;

	// bottom left
	p1.x = (double)(l_map_pixel_data[row][col].x);
	p1.y = (double)(l_map_pixel_data[row][col].y);

	// bottom right
	p2.x = (double)(l_map_pixel_data[row][col + 1].x);
	p2.y = (double)(l_map_pixel_data[row][col + 1].y);

	// top left
	p3.x = (double)(l_map_pixel_data[row - 1][col].x);
	p3.y = (double)(l_map_pixel_data[row - 1][col].y);

	// top right
	p4.x = (double)(l_map_pixel_data[row - 1][col + 1].x);
	p4.y = (double)(l_map_pixel_data[row - 1][col + 1].y);

	double x_scalar = (lon - l_map_col_lon_data[col]) / 10.0;	// 10.0f is the longitudinal step in our array
	double y_scalar = (lat - l_map_row_lat_data[row]) / 5.0;		// 5.0f is the latitudinal step in our array

	SVS::Vector2Dd p1_to_p2 = p2 - p1;
	SVS::Vector2Dd p3_to_p4 = p4 - p3;

	SVS::Vector2Dd v1 = p1_to_p2 * x_scalar;	// interpolated longitude from p1 to p2
	SVS::Vector2Dd v2 = p3_to_p4 * x_scalar;	// interpolated longitude from p3 to p4

	SVS::Vector2Dd v3 = ((p3 + v2) - (p1 + v1)) * y_scalar;	// interpolated attitude

	SVS::Vector2Dd pixel_location = p1 + v1 + v3;

	pixel_data->x = (int)(std::round(pixel_location.x));
	pixel_data->y = (int)(std::round(pixel_location.y));
}
