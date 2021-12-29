#include "model3d_impl.h"

#include "affine_transform.h"
#include "draw_lines.h"

void model3d_impl::draw(
	const HDC& dc,
	matrix<double>& world_to_projection_matrix,
	const point_double point_world0_in_screen,
	const point_double pixel_density
)
{
	
	const auto temp = new matrix<double>(verticies_);
	const auto projection_matrix = new matrix<double>(world_to_projection_matrix * *temp);

	const auto size_n = new size_t(edges_.n());

	for (size_t i = 1; i < *size_n; i++)
	{
		for (size_t j = i; j < *size_n; j++)
		{

			if (edges_[i][j] == 1)
			{
				const double* first_point_k = new double(1 / (*projection_matrix)[2][i]);
				const double* second_point_k = new double(1 / (*projection_matrix)[2][j]);
				draw_lines::connect_points(
					dc,
					point_double{ (*projection_matrix)[0][i] * *first_point_k, (*projection_matrix)[1][i] * *first_point_k },
					point_double{ (*projection_matrix)[0][j] * *second_point_k, (*projection_matrix)[1][j] * *second_point_k },
					point_world0_in_screen,
					pixel_density
				);

				/*draw_lines::connect_points(
					dc,
					point_double{ (*projection_matrix)[0][i], (*projection_matrix)[1][i]},
					point_double{ (*projection_matrix)[0][j], (*projection_matrix)[1][j]},
					point_world0_in_screen,
					pixel_density
				);*/

				delete first_point_k;
				delete second_point_k;

			}

		}
	}

	delete temp;
	delete projection_matrix;
	delete size_n;

}

void model3d_impl::resize(HDC& dc, const point_double_3d new_size)
{
	auto current_position = position_;
	
	verticies_ = affine_transform::scale(verticies_, new_size);
}

void model3d_impl::move(HDC& dc, const point_double_3d vector)
{
	position_ += vector;
	verticies_ = affine_transform::move(verticies_, vector);

}

void model3d_impl::rotate(
	HDC& dc, 
	const rotation_type_3d rotation_type, 
	const double radian
)
{
	if (rotation_type == rotation_type_3d::self_abscissa || rotation_type == rotation_type_3d::self_ordinate || rotation_type == rotation_type_3d::self_applicate)
	{

		point_double_3d move_point = position_;

		move_point += get_center();

		verticies_ = affine_transform::move(verticies_, -move_point);
		switch (rotation_type)
		{
			case rotation_type_3d::self_abscissa:
				{
					verticies_ = affine_transform::rotate(verticies_, rotation_type_3d::abscissa, radian);
				}
				break;

			case rotation_type_3d::self_ordinate:
				{
					verticies_ = affine_transform::rotate(verticies_, rotation_type_3d::ordinate, radian);
				}
				break;

			case rotation_type_3d::self_applicate:
				{
					verticies_ = affine_transform::rotate(verticies_, rotation_type_3d::applicate, radian);
				}
				break;
		}
		verticies_ = affine_transform::move(verticies_, move_point);

	} else
	{
		verticies_ = affine_transform::rotate(verticies_, rotation_type, radian);
	}

	
}

void model3d_impl::mapping(
	HDC& dc, 
	const mirror_type_3d mirror_type
)
{

	verticies_ = affine_transform::mirror(verticies_, mirror_type);

}


matrix<int> model3d_impl::get_edges()
{
	const size_t n = verticies_.m();
	const size_t m = faces_.n();


	matrix<int> edges_matrix(n, n);

	for (size_t i = 0; i < m; i++)
	{
		edges_matrix[ faces_[i][0] - 1][ faces_[i][1] - 1] = 1;
		edges_matrix[ faces_[i][1] - 1][ faces_[i][0] - 1] = 1;
		edges_matrix[ faces_[i][1] - 1][ faces_[i][2] - 1] = 1;
		edges_matrix[ faces_[i][2] - 1][ faces_[i][1] - 1] = 1;
		edges_matrix[ faces_[i][2] - 1][ faces_[i][0] - 1] = 1;
		edges_matrix[ faces_[i][0] - 1][ faces_[i][2] - 1] = 1;
	}

	/*std::fstream out("output.txt");

	logger::log(out, edges_matrix, "edges");*/

	edges_matrix = matrix<int>(std::vector<std::vector<int>>{
		{ 0,	1,	1,	1,	1,	1,	1,	1 },
		{ 1,	0,	0,	1,	1,	1,	1,	1 },
		{ 1,	0,	0,	1,	0,	1,	0,	1 },
		{ 1,	1,	1,	0,	0,	1,	1,	1 },
		{ 1,	1,	0,	0,	0,	1,	0,	1 },
		{ 1,	1,	1,	1,	1,	0,	0,	1 },
		{ 1,	1,	0,	1,	0,	0,	0,	0 },
		{ 1,	1,	1,	1,	1,	1,	0,	0 }
	});

	return edges_matrix;

}

void model3d_impl::custom_affine(HDC& dc)
{

	

}
