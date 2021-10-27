#include "camera_2d.h"

#include "convert_fucntions.h"
#include "graph_2d.h"
#include "PointDouble.h"

void camera_2d::draw(HDC dc, graph_2d graph)
{
	PointDouble drawp{
		p_w0.x,
		p_w0.y
	};

	drawp = convert_functions::world_to_screen(
			drawp,
			L,
			R,
			T,
			B,
			W,
			H
	);

	MoveToEx(
		dc,
		drawp.x,
		drawp.y,
		nullptr
	);

	

	graph.draw(dc, W, H, L, R, T, B, px_x, px_y);

}

int camera_2d::resize(SIZE new_size, graph_2d graph)
{

	return -1;

}

int camera_2d::move_to(POINT new_point, graph_2d graph)
{

	return -1;

}

void camera_2d::on_window_size_change(int _W, int _H)
{

	W = _W;
	H = _H;

	double buff_R = R;
	double multiplier = W/H;

	R = multiplier * (T - B) + L;
	L = -1 * multiplier * (T - B) + buff_R;

	px_x = abs(static_cast<int>((R - L) / W));
	px_y = abs(static_cast<int>((T - B) / H));

}

bool camera_2d::is_inner_point(POINT point, graph_2d graph)
{

	return false;

}

std::string camera_2d::to_string()
{

	return
		"camera_2d";

}
