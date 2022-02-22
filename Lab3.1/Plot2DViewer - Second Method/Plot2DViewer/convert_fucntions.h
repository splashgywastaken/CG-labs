#ifndef CONVERT_FUNCTIONS
#define CONVERT_FUNCTIONS

#include "Camera2D.h"
#include "PointDouble.h"

class convert_functions
{

public:

	static point_double world_to_screen(
		const point_double point_world,
		const point_double point_world0_in_screen,
		const point_double pixel
	)
	{

		point_double result;

		result.x = point_world0_in_screen.x + point_world.x * pixel.x;
		result.y = point_world0_in_screen.y - point_world.y * pixel.y;

		return result;

	}

	static point_double screen_to_world(
		const POINT point_screen,
		const point_double point_world0_in_screen,
		const point_double pixel
	)
	{

		point_double result;

		result.x = (point_screen.x - point_world0_in_screen.x + 0.5) / pixel.x;
		result.y = -(point_screen.y - point_world0_in_screen.y + 0.5) / pixel.y;

		return result;

	}

};

#endif
