#ifndef DRAW_LINES_H
#define DRAW_LINES_H
#include <Windows.h>
#include "PointDouble.h"
#include "convert_fucntions.h"

class draw_lines
{
public:
	static void line_to(
		const HDC dc,
		const point_double world_point,
		const point_double point_world0_in_screen,
		const point_double pixel)
	{

		const point_double point_screen = convert_functions::world_to_screen(
			world_point,
			point_world0_in_screen,
			pixel
		);

		line_to(dc, point_screen);

	}

	static void move_to(
		const HDC dc,
		const point_double world_point,
		const point_double point_world0_in_screen,
		const point_double pixel)
	{

		const point_double screen_point = convert_functions::world_to_screen(
			world_point,
			point_world0_in_screen,
			pixel
		);

		move_to(dc, screen_point);

	}

	static void connect_points(
		const HDC& dc,
		const point_double& first,
		const point_double& second,
		const point_double& point_world0,
		const point_double& pixel
	)
	{

		const point_double screen_first = convert_functions::world_to_screen(
			first,
			point_world0,
			pixel
		);
		const point_double screen_second = convert_functions::world_to_screen(
			second,
			point_world0,
			pixel
		);

		connect_points(
			dc,
			first,
			second
		);

	}

private:

	static void connect_points(
		const HDC dc,
		const POINT first,
		const POINT second
	)
	{

		MoveToEx(dc, first.x, first.y, nullptr);
		LineTo(dc, second.x, second.y);

	}

	static void connect_points(
		const HDC& dc,
		const point_double& first,
		const point_double& second
	)
	{

		MoveToEx(dc, first.x, first.y, nullptr);
		LineTo(dc, second.x, second.y);

	}

	static void move_to(const HDC dc, const point_double screen_point)
	{

		MoveToEx(dc, screen_point.x, screen_point.y, nullptr);

	}
	
	static void line_to(const HDC dc, const point_double screen_point)
	{

		LineTo(dc, screen_point.x, screen_point.y);
		MoveToEx(dc, screen_point.x, screen_point.y, nullptr);

	}

};
#endif