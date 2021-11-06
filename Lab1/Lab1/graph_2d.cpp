#include "graph_2d.h"
#include "convert_fucntions.h"
#include "PointDouble.h"

#include <Windows.h>
#include <algorithm>
#include <cctype>
#include <string>

void graph_2d::set_graph_type(std::string data, double _x_param, double _y_param)
{

	if (!data.empty()) {

		x_param = _x_param;
		y_param = _y_param;

		std::transform(data.begin(), data.end(), data.begin(),
		[](unsigned char c) { return std::tolower(c); });

		if (data == "astroida") {

			graph_type = ASTROIDA;

		}
		else if (data == "ellipse") {

			graph_type = ELLIPSE;

		}
		else if (data == "cicloida") {

			graph_type = CICLOIDA;

		}
		else if (data == "parabola")
		{
			graph_type = PARABOLA;
		}
		else if (data == "rose")
		{
			graph_type = ROSE;
		}
		else
		{

			LPCWSTR title = (LPCWSTR)"Your graph type was wrong";
			LPCWSTR text = (LPCWSTR)"INPUT ERROR";

			MessageBox(
				GetActiveWindow(),
				title,
				text,
				MB_ICONERROR
			);

			PostQuitMessage(0);

		}

	}
}

void graph_2d::draw(HDC dc, PointDouble point_world0, double pixel_y, double pixel_x, double L, double R) const
{


	PointDouble draw_point_world{
		pos_x,
		pos_y
	};

	POINT draw_point_screen{
		0,
		0
	};

	switch (graph_type){

			case ELLIPSE:
			{
				const double step = static_cast<double>(1) / pixel_x;
				double i = L + step;

				draw_point_world.x = this->x_param * cos(i);
				draw_point_world.y = this->y_param * sin(i);

				draw_point_screen = convert_functions::world_to_screen(
					draw_point_world, point_world0, pixel_x, pixel_y
				);

				MoveToEx(
					dc, draw_point_screen.x, draw_point_screen.y, nullptr
				);

				for (i; i < R + step; i+=step)
				{

					draw_point_world.x = this->x_param * cos(i);
					draw_point_world.y = this->y_param * sin(i);

					draw_point_screen = convert_functions::world_to_screen(
						draw_point_world, point_world0, pixel_x, pixel_y
					);

					LineTo(
						dc, draw_point_screen.x, draw_point_screen.y
					);
					MoveToEx(
						dc, draw_point_screen.x, draw_point_screen.y, nullptr
					);

				}

			}
			break;

			case ASTROIDA:
			{

				const double step = static_cast<double>(1) / pixel_x;
				double i = L + step;

				draw_point_world.x = this->x_param * pow(cos(i), 3);
				draw_point_world.y = this->y_param * pow(sin(i), 3);

				draw_point_screen = convert_functions::world_to_screen(
					draw_point_world, point_world0, pixel_x, pixel_y
				);

				MoveToEx(
					dc, draw_point_screen.x, draw_point_screen.y, nullptr
				);

				for (i; i < R + step; i += step)
				{

					draw_point_world.x = this->x_param * pow(cos(i), 3);
					draw_point_world.y = this->y_param * pow(sin(i), 3);

					draw_point_screen = convert_functions::world_to_screen(
						draw_point_world, point_world0, pixel_x, pixel_y
					);

					LineTo(
						dc, draw_point_screen.x, draw_point_screen.y
					);
					MoveToEx(
						dc, draw_point_screen.x, draw_point_screen.y, nullptr
					);

				}

			}
			break;

			case CICLOIDA:
			{

				const double step = static_cast<double>(1) / pixel_x;
				double i = L + step;

				draw_point_world.x = this->x_param * (i - sin(i));
				draw_point_world.y = this->y_param * (1 - cos(i));

				draw_point_screen = convert_functions::world_to_screen(
					draw_point_world, point_world0, pixel_x, pixel_y
				);

				MoveToEx(
					dc, draw_point_screen.x, draw_point_screen.y, nullptr
				);

				for (i; i < R + step; i += step)
				{

					draw_point_world.x = this->x_param * (i - sin(i));
					draw_point_world.y = this->y_param * (1 - cos(i));

					draw_point_screen = convert_functions::world_to_screen(
						draw_point_world, point_world0, pixel_x, pixel_y
					);

					LineTo(
						dc, draw_point_screen.x, draw_point_screen.y
					);
					MoveToEx(
						dc, draw_point_screen.x, draw_point_screen.y, nullptr
					);

				}

			}
			break;

			case PARABOLA:
			{

				const double step = static_cast<double>(1) / pixel_x;
				double i = L + step;

				draw_point_world.x = this->x_param * i;
				draw_point_world.y = this->y_param * (draw_point_world.x * draw_point_world.x);

				draw_point_screen = convert_functions::world_to_screen(
					draw_point_world, point_world0, pixel_x, pixel_y
				);

				MoveToEx(
					dc, draw_point_screen.x, draw_point_screen.y, nullptr
				);

				for (i; i < R + step; i += step)
				{

					draw_point_world.x = this->x_param * i;
					draw_point_world.y = this->y_param * (draw_point_world.x * draw_point_world.x);

					draw_point_screen = convert_functions::world_to_screen(
						draw_point_world, point_world0, pixel_x, pixel_y
					);

					LineTo(
						dc, draw_point_screen.x, draw_point_screen.y
					);
					MoveToEx(
						dc, draw_point_screen.x, draw_point_screen.y, nullptr
					);

				}

			}
			break;

			case ROSE:
			{
				
				const double step = static_cast<double>(1) / pixel_x;
				double i = L + step;

				PointDouble draw_point_polar;

				draw_point_polar.x = this->x_param * i;
				draw_point_polar.y = this->y_param * cos(3 * draw_point_polar.x);

				draw_point_world.x = draw_point_polar.y * cos(draw_point_polar.x);
				draw_point_world.y = draw_point_polar.y * sin(draw_point_polar.x);

				draw_point_screen = convert_functions::world_to_screen(
					draw_point_world, point_world0, pixel_x, pixel_y
				);

				MoveToEx(
					dc, draw_point_screen.x, draw_point_screen.y, nullptr
				);

				for (i; i < R + step; i += step)
				{

					draw_point_polar.x = this->x_param * i;
					draw_point_polar.y = this->y_param * cos(3 * draw_point_polar.x);

					draw_point_world.x = draw_point_polar.y * cos(draw_point_polar.x);
					draw_point_world.y = draw_point_polar.y * sin(draw_point_polar.x);

					draw_point_screen = convert_functions::world_to_screen(
						draw_point_world, point_world0, pixel_x, pixel_y
					);

					LineTo(
						dc, draw_point_screen.x, draw_point_screen.y
					);
					MoveToEx(
						dc, draw_point_screen.x, draw_point_screen.y, nullptr
					);

				}

			}
			break;
	}
}

void graph_2d::move(POINT new_point)
{

	pos_x = new_point.x;
	pos_y = new_point.y;

}
