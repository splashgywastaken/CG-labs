#include "rectangle.h"

void rectangle::transform(HDC& hdc, const int& coordinate_x, const int& coordinate_y, const int& size_x, const int& size_y)
{
}

void rectangle::move(int offsetX, int offsetY)
{

	offsetX += get_coordinate_x();
	offsetY += get_coordinate_y();

	set_coordinate_x(offsetX);
	set_coordinate_y(offsetY);

}

void rectangle::resize(int resize_param)
{
	int size_x = get_size_x() + resize_param;
	int size_y = get_size_y() + resize_param;

	set_size_x(size_x);
	set_size_y(size_y);

}

int rectangle::create(
	HDC& hdc
)
{
	MoveToEx(hdc, get_coordinate_x(), get_coordinate_y(), nullptr);
	LineTo(hdc, get_coordinate_x() + get_size_x(), get_coordinate_y());
	LineTo(hdc, get_coordinate_x() + get_size_x(), get_coordinate_y() + get_size_y());
	MoveToEx(hdc, get_coordinate_x() + get_size_x(), get_coordinate_y() + get_size_y(), nullptr);
	LineTo(hdc, get_coordinate_x(), get_coordinate_y() + get_size_y());
	LineTo(hdc, get_coordinate_x(), get_coordinate_y());

	return 1;
}

void rectangle::create(HDC& hdc, const int& coordinate_first_x, const int& coordinate_first_y,
	const int& coordinate_second_x, const int& coordinate_second_y)
{
}

void rectangle::clear(HDC dc)
{
}

void rectangle::create(HDC& hdc, const int& coordinate_x, const int& coordinate_y)
{
}

void rectangle::set_offsets(int coordinates[2][4], const std::vector<POINT>& mouse_pos)
{
	for (int j = 0; j < 4; j++)
		{
			coordinates[0][j] += mouse_pos.at(0).x;
			coordinates[1][j] += mouse_pos.at(1).y;
		}
}

