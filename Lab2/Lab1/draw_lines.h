#ifndef DRAW_LINES_H
#define DRAW_LINES_H
#include "framework.h"
#include "PointDouble.h"
#include "convert_fucntions.h"

class draw_lines
{
public:
	static void line_to(HDC dc, point_double world_point, POINT point_world0_in_screen, point_double pixel)
	{

		const POINT point_screen = convert_functions::world_to_screen(
			world_point,
			point_world0_in_screen,
			pixel
		);

		line_to(dc, point_screen);

	}

	static void move_to(HDC dc, point_double world_point, POINT point_world0_in_screen, point_double pixel)
	{

		const POINT screen_point = convert_functions::world_to_screen(
			world_point,
			point_world0_in_screen,
			pixel
		);

		move_to(dc, screen_point);

	}

	static void connect_points(
		HDC dc,
		const point_double first,
		const point_double second,
		const POINT point_world0,
		const point_double pixel
	)
	{

		const POINT screen_first = convert_functions::world_to_screen(
			first,
			point_world0,
			pixel
		);
		const POINT screen_second = convert_functions::world_to_screen(
			second,
			point_world0,
			pixel
		);

		connect_points(
			dc,
			screen_first,
			screen_second
		);

	}

private:

	static void connect_points(
		HDC dc,
		POINT first,
		POINT second
	)
	{

		MoveToEx(dc, first.x, first.y, nullptr);
		LineTo(dc, second.x, second.y);

	}

	static void move_to(HDC dc, POINT screen_point)
	{

		MoveToEx(dc, screen_point.x, screen_point.y, nullptr);

	}
	
	static void line_to(HDC dc, POINT screen_point)
	{

		LineTo(dc, screen_point.x, screen_point.y);
		MoveToEx(dc, screen_point.x, screen_point.y, nullptr);

	}

};
#endif