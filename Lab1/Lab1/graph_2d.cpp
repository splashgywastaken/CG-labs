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
		else
		{

			MessageBox(
				GetActiveWindow(),
				LPCWSTR("Your graph type was wrong"),
				LPCWSTR("INPUT ERROR"),
				MB_ICONERROR
			);

			PostQuitMessage(0);

		}

	}
}

void graph_2d::draw(HDC dc, int W , int H, double L, double R, double T, double B, double px_x, double px_y)
{


	PointDouble draw_p{
		pos_x,
		pos_y
	};

	switch (graph_type){

			case ELLIPSE:
			{

				draw_p.x = this->x_param * cos(1);
				draw_p.y = this->y_param * sin(1);

				draw_p = convert_functions::world_to_screen(draw_p, L, R, T, B, W, H);

				MoveToEx(dc, draw_p.x + pos_x, draw_p.y + pos_y, nullptr);

				for (int i = 2; i < W; i++)
				{

					draw_p.x = this->x_param * cos(i);
					draw_p.y = this->y_param * sin(i);

					draw_p = convert_functions::world_to_screen(draw_p, L, R, T, B, W, H);

					LineTo(dc, draw_p.x + pos_x, draw_p.y + pos_y);
					MoveToEx(dc, draw_p.x + pos_x, draw_p.y + pos_y, nullptr);

				}

			}
			break;

			case ASTROIDA:
			{

				draw_p.x = this->x_param * pow(cos(1), 3);
				draw_p.y = this->y_param * pow(sin(1), 3);

				draw_p = convert_functions::world_to_screen(draw_p, L, R, T, B, W, H);

				MoveToEx(dc, draw_p.x + pos_x, draw_p.y + pos_y, nullptr);

				for (int i = 2; i < W; i++)
				{

					draw_p.x = this->x_param * pow(cos(i), 3);
					draw_p.y = this->y_param * pow(sin(i), 3);
					
					draw_p = convert_functions::world_to_screen(draw_p, L, R, T, B, W, H);

					LineTo(dc, draw_p.x, draw_p.y);
					MoveToEx(dc, draw_p.x, draw_p.y, nullptr);

				}

			}
			break;

			case CICLOIDA:
			{

				draw_p.x = this->x_param * (1 - sin(1));
				draw_p.y = this->y_param * (1 - cos(1));

				draw_p = convert_functions::world_to_screen(draw_p, L, R, T, B, W, H);

				MoveToEx(dc, draw_p.x + pos_x, draw_p.y + pos_y, nullptr);

				for (int i = 2; i < W; i++)
				{

					draw_p.x = this->x_param * ( i - sin(i) );
					draw_p.y = this->y_param * ( 1 - cos(i) );

					draw_p = convert_functions::world_to_screen(draw_p, L, R, T, B, W, H);

					LineTo(dc, draw_p.x, draw_p.y);
					MoveToEx(dc, draw_p.x, draw_p.y, nullptr);

				}

			}
			break;
	}
}
