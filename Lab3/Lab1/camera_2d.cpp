#include "camera_2d.h"

#include "convert_fucntions.h"
#include "graph_2d.h"

void camera_2d::draw(HDC dc, graph_2d graph)
{

	//axis(dc);

	graph.draw(
		dc,
		point_world0_in_screen_,
		point_double{
			pixel_x_,
			pixel_y_
		},
		l_,
		r_
	);

}

void camera_2d::draw(HDC dc, model* object) const
{

	axis(dc);

	object->draw(
		dc,
		point_world0_in_screen_,
		point_double{pixel_x_, pixel_y_},
		l_,
		r_
	);

}

int camera_2d::resize(double new_size, POINT screen_point)
{

	point_world0_in_screen_.x = static_cast<LONG>(
		static_cast<double>(screen_point.x) - new_size * (static_cast<double>(screen_point.x) - static_cast<double>(point_world0_in_screen_.x))
			);
	point_world0_in_screen_.y = static_cast<LONG>(
		static_cast<double>(screen_point.y) - new_size * (static_cast<double>(screen_point.y) - static_cast<double>(point_world0_in_screen_.y))
			);

	pixel_x_ = pixel_x_ * new_size;
	pixel_y_ = pixel_y_ * new_size;

	return 1;

}

int camera_2d::move_to(POINT new_point, graph_2d graph)
{

	graph.move(new_point);

	return 1;

}

void camera_2d::on_window_size_change(int _W, int _H)
{
	const double W0 = w_;
	const double H0 = h_;

	w_ = _W;
	h_ = _H;

	const double multiplierW = static_cast<double>(w_) / W0;
	const double multiplierH = static_cast<double>(h_) / H0;
	const double multiplier_px = pixel_x_ / pixel_y_;

	point_world0_in_screen_.x = 
		static_cast<LONG>(
			multiplierW * static_cast<double>(point_world0_in_screen_.x)
			);
	point_world0_in_screen_.y =
		static_cast<LONG>(
			static_cast<double>(_H) / static_cast<double>(2) * (multiplierH - multiplierW * multiplier_px)
			+
			static_cast<double>(point_world0_in_screen_.y) * multiplierW * multiplier_px
			);

	pixel_x_ = multiplierW * pixel_x_;
	pixel_y_ = pixel_x_;

}

void camera_2d::clear_window(HDC dc)
{

	RECT r;
	GetClientRect(WindowFromDC(dc), &r);
	Rectangle(dc, -1, -1, r.right, r.bottom);

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

void camera_2d::axis(const HDC dc) const
{

	//«адаем координаты дл€ точки в мировых координатах
	point_double point_world{
		static_cast<int>(get_l() - 1),
		0
	};
	POINT point_screen = convert_functions::world_to_screen(
		point_world,
		point_world0_in_screen_,
		point_double{
			pixel_x_,
			pixel_y_
		}
	);
	//ѕереходим в левый край по центру
	MoveToEx(
		dc,
		point_screen.x,
		point_screen.y,
		nullptr
		);

	point_world.x = get_r() + 1;
	point_screen = convert_functions::world_to_screen(
		point_world,
		point_world0_in_screen_,
		point_double{
			pixel_x_,
			pixel_y_
		}
	);
	//ѕрорисовываем линию до правого кра€ 
	LineTo(
		dc,
		point_screen.x,
		point_screen.y
		);

	point_world.x = 0;
	point_world.y = get_t() + 1;
	point_screen = convert_functions::world_to_screen(
		point_world,
		point_world0_in_screen_,
		point_double{
			pixel_x_,
			pixel_y_
		}
	);
	//ѕередвигаем точку в верхний край
	MoveToEx(
		dc,
		point_screen.x,
		point_screen.y,
		nullptr
	);
	point_world.y = get_b() - 1;
	point_screen = convert_functions::world_to_screen(
		point_world,
		point_world0_in_screen_,
		point_double{
			pixel_x_,
			pixel_y_
		}
	);
	//ѕрорисовываем линию до нижнего кра€
	LineTo(
		dc,
		point_screen.x,
		point_screen.y
	);

}
