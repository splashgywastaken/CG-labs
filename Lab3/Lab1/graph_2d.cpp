#include "graph_2d.h"
#include "convert_fucntions.h"
#include "PointDouble.h"

#include <Windows.h>
#include <algorithm>
#include <cctype>
#include <string>

#include "draw_lines.h"

void graph_2d::set_graph_type(std::string data, point_double params)
{

	if (!data.empty()) {

		params_.x = params.x;
		params_.y = params.y;

		std::transform(data.begin(), data.end(), data.begin(),
		[](unsigned char c) { return std::tolower(c); });

		if (data == "astroida") {

			graph_type_ = ASTROIDA;

		}
		else if (data == "ellipse") {

			graph_type_ = ELLIPSE;

		}
		else if (data == "cicloida") {

			graph_type_ = CICLOIDA;

		}
		else if (data == "parabola")
		{
			graph_type_ = PARABOLA;
		}
		else if (data == "rose")
		{
			graph_type_ = ROSE;
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

void graph_2d::draw(HDC dc, point_double point_world0_in_screen, point_double pixel, double L, double R) const
{


	point_double draw_point_world{
		position_
	};

	POINT draw_point_screen{
		0,
		0
	};

	switch (graph_type_){

			case ELLIPSE:
			{
				const double step = static_cast<double>(1) / pixel.x;
				double i = L + step;

				draw_point_world.x = this->params_.x * cos(i);
				draw_point_world.y = this->params_.y * sin(i);

				draw_lines::move_to(
						dc,
						draw_point_world,
							point_world0_in_screen,
						point_double{
							pixel
						}
				);

				for (i; i < R + step; i+=step)
				{

					draw_point_world.x = this->params_.x * cos(i);
					draw_point_world.y = this->params_.y * sin(i);

					draw_point_screen = convert_functions::world_to_screen(
						draw_point_world, point_world0_in_screen, pixel
					);

					draw_lines::line_to(
						dc,
						draw_point_world,
							point_world0_in_screen,
						point_double{
							pixel
						}
					);

				}

			}
			break;

			case ASTROIDA:
			{

				const double step = static_cast<double>(1) / pixel.x;
				double i = L + step;

				draw_point_world.x = this->params_.x * pow(cos(i), 3);
				draw_point_world.y = this->params_.y * pow(sin(i), 3);

				draw_lines::move_to(
					dc,
					draw_point_world,
						point_world0_in_screen,
					point_double{
						pixel
					}
				);

				for (i; i < R + step; i += step)
				{

					draw_point_world.x = this->params_.x * pow(cos(i), 3);
					draw_point_world.y = this->params_.y * pow(sin(i), 3);

					draw_point_screen = convert_functions::world_to_screen(
						draw_point_world, point_world0_in_screen, pixel
					);

					draw_lines::line_to(
						dc,
						draw_point_world,
							point_world0_in_screen,
						point_double{
							pixel
						}
					);


				}

			}
			break;

			case CICLOIDA:
			{

				const double step = static_cast<double>(1) / pixel.x;
				double i = L + step;

				draw_point_world.x = this->params_.x * (i - sin(i));
				draw_point_world.y = this->params_.y * (1 - cos(i));

				draw_lines::move_to(
					dc,
					draw_point_world,
						point_world0_in_screen,
					point_double{
						pixel
					}
				);

				for (i; i < R + step; i += step)
				{

					draw_point_world.x = this->params_.x * (i - sin(i));
					draw_point_world.y = this->params_.y * (1 - cos(i));

					draw_lines::line_to(
							dc,
							draw_point_world,
								point_world0_in_screen,
							point_double{
								pixel
						}
					);

				}

			}
			break;

			case PARABOLA:
			{

				const double step = static_cast<double>(1) / pixel.x;
				double i = L + step;

				draw_point_world.x = this->params_.x * i;
				draw_point_world.y = this->params_.y * (draw_point_world.x * draw_point_world.x);

				draw_point_screen = convert_functions::world_to_screen(
					draw_point_world, point_world0_in_screen, pixel
				);

				MoveToEx(
					dc, draw_point_screen.x, draw_point_screen.y, nullptr
				);

				for (i; i < R + step; i += step)
				{

					draw_point_world.x = this->params_.x * i;
					draw_point_world.y = this->params_.y * (draw_point_world.x * draw_point_world.x);

					draw_lines::line_to(
						dc,
						draw_point_world,
							point_world0_in_screen,
						point_double{
							pixel
						}
					);

				}

			}
			break;

			case ROSE:
			{
				
				const double step = static_cast<double>(1) / pixel.x;
				double i = L + step;

				point_double draw_point_polar;

				draw_point_polar.x = this->params_.x * i;
				draw_point_polar.y = this->params_.y * cos(3 * draw_point_polar.x);

				draw_point_world.x = draw_point_polar.y * cos(draw_point_polar.x);
				draw_point_world.y = draw_point_polar.y * sin(draw_point_polar.x);

				draw_lines::move_to(
						dc,
						draw_point_world,
							point_world0_in_screen,
						point_double{
							pixel
						}
				);

				for (i; i < R + step; i += step)
				{

					draw_point_polar.x = this->params_.x * i;
					draw_point_polar.y = this->params_.y * cos(3 * draw_point_polar.x);

					draw_point_world.x = draw_point_polar.y * cos(draw_point_polar.x);
					draw_point_world.y = draw_point_polar.y * sin(draw_point_polar.x);

					draw_lines::line_to(
						dc, 
						draw_point_world,
							point_world0_in_screen,
						point_double{
							pixel
						});

				}

			}
			break;
	}
}

void graph_2d::move(const POINT new_point)
{

	position_ = new_point;

}
