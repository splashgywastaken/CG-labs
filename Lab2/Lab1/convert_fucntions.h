#ifndef CONVERT_FUNCTIONS
#define CONVERT_FUNCTIONS

#include "PointDouble.h"
#include "framework.h"

static class convert_functions
{

public:

	static POINT world_to_screen(point_double point_world, POINT point_world0_in_screen, point_double pixel)
	{

		POINT result{};

		result.x = static_cast<LONG>(point_world0_in_screen.x + pixel.x * point_world.x);
		result.y = static_cast<LONG>(point_world0_in_screen.y - pixel.y * point_world.y);

		return result;

	}

	static point_double screen_to_world(POINT point_screen, POINT point_world0_in_screen, point_double pixel)
	{

		point_double result;

		result.x = (point_screen.x - point_world0_in_screen.x + 0.5) / pixel.x;
		result.y = -(point_screen.y - point_world0_in_screen.y + 0.5) / pixel.y;

		return result;

	}
	
};

#endif
