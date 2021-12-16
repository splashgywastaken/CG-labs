#include "model_2d.h"

#include "affine_transform.h"
#include "draw_lines.h"
#include "PointDouble.h"

void model_2d::draw(
	HDC& dc,
	const point_double point0_in_world,
	const point_double pixel,
	double l,
	double r
)
{

	const size_t m = edges_.n();

	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			//Отрисовка
			if (edges_[i][j] == 1.0)
			{

				const double k_1 = 1 / verticies_[i][2];
				const double k_2 = 1 / verticies_[j][2];

				draw_lines::connect_points(
					dc,
					point_double{ verticies_[i][0] * k_1, verticies_[i][1] * k_1 },
					point_double{ verticies_[j][0] * k_2, verticies_[j][1] * k_2 },
					point0_in_world,
					pixel
				);

			}

		}
	}

}

void model_2d::resize(const HDC dc, const point_double new_size)
{

	//verticies_ = affine_transform::scaling(new_size, verticies_);
	custom_resize(dc, new_size);
}

void model_2d::custom_resize(HDC dc, const point_double new_size)
{
	verticies_ = affine_transform::custom_scaling(
		new_size,
		rotation_radian_,
		previous_point_,
		centre_point_,
		verticies_
	);
}

void model_2d::move(HDC dc, const point_double vector)
{

	verticies_ = affine_transform::parallel_move_by_vector(vector, verticies_);
	previous_point_ = affine_transform::parallel_move_by_vector(vector, previous_point_);

}

void model_2d::rotate(HDC dc, const POINT rotation_type)
{

	verticies_ = affine_transform::rotation( rotation_type, verticies_);



}



void model_2d::rotate(const HDC dc, const double radian)
{

	//verticies_ = affine_transform::rotation(radian, verticies_);
	rotation_radian_ += radian;
	custom_rotate(dc, radian);
}

void model_2d::custom_rotate(HDC dc, const double radian) 
{
	verticies_ = affine_transform::custom_rotation(
		radian,
		previous_point_,
		centre_point_,
		verticies_
	);
}

void model_2d::rotate(HDC dc, double cos, double sin)
{



}

void model_2d::custom_affine(HDC dc)
{



}

point_double model_2d::get_centre()
{
	return
	point_double{
		get_avg_x(),
		get_avg_y()
	};
}

double model_2d::get_avg_x()
{

	double x = 0;
	const size_t n = verticies_.n();

	for (size_t i = 0; i < n; i++)
	{
		x += verticies_[i][0] / verticies_[i][2];
	}

	return x / n;

}

double model_2d::get_avg_y()
{

	double y = 0;
	const size_t n = verticies_.n();

	for (size_t i = 0; i < n; i++)
	{
		y += verticies_[i][1] / verticies_[i][2];
	}

	return y / n;

}

double model_2d::get_fat_right()
{
	double far_right = verticies_[0][0] / verticies_[0][2];
	for (size_t i = 1; i < verticies_.n(); i++)
	{
		const double x = verticies_[i][0] / verticies_[i][2];
		if (x >= far_right)
			far_right = x;
	}
	return far_right;	
}

double model_2d::get_far_left()
{
	double far_left = verticies_[0][0] / verticies_[0][2];
	for (size_t i = 1; i < verticies_.n(); i++)
	{
		const double x = verticies_[i][0] / verticies_[i][2];
		if (x <= far_left)
			far_left = x;
	}
	return far_left;
}

double model_2d::get_far_top()
{
	double far_top = verticies_[0][1] / verticies_[0][2];
	for (size_t i = 1; i < verticies_.n(); i++)
	{
		const double y = verticies_[i][1] / verticies_[i][2];
		if (y >= far_top)
			far_top = y;
	}
	return far_top;
}

double model_2d::get_far_bottom()
{
	double far_bottom = verticies_[0][1] / verticies_[0][2];
	for (size_t i = 1; i < verticies_.n(); i++)
	{
		const double y = verticies_[i][1] / verticies_[i][2];
		if (y <= far_bottom)
			far_bottom = y;
	}
	return far_bottom;
}
