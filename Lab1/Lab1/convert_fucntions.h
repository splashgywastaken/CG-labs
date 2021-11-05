#ifndef CONVERT_FUNCTIONS
#define CONVERT_FUNCTIONS

#include "PointDouble.h"
#include "framework.h"

static class convert_functions
{

public:

	static POINT world_to_screen(PointDouble point_world, PointDouble point_world0_in_screen, double pixel_x, double pixel_y)
	{

		POINT result{};

		result.x = static_cast<LONG>(point_world0_in_screen.x + pixel_x * point_world.x);
		result.y = static_cast<LONG>(point_world0_in_screen.y - pixel_y * point_world.y);

		return result;

	}

	static PointDouble screen_to_world(POINT point_screen, PointDouble point_world0_in_screen, double pixel_x, double pixel_y)
	{

		PointDouble result;

		result.x = (point_screen.x - point_world0_in_screen.x + 0.5) / pixel_x;
		result.y = -(point_screen.y - point_world0_in_screen.y + 0.5) / pixel_y;

		return result;

	}
	
};

#endif
