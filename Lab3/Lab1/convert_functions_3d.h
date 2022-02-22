#ifndef CONVERT_FUNCTIONS_3D_H
#define CONVERT_FUNCTIONS_3D_H
#include "Matrix.h"
#include "PointDouble.h"
#include "point_double_3d.h"
#include "vector_3d.h"
#include "fstream"
#include "Logger.h"

class convert_functions_3d
{
	static matrix<double> convert_world_to_view(
		matrix<double> input_matrix,
		const vector_3d& vector_i,
		const vector_3d& vector_j,
		const vector_3d& vector_k,
		const point_double_3d point_ov
		)
	{
		matrix<double> affine_matrix(std::vector<std::vector<double>>{
			{vector_i.x(), vector_i.y(), vector_i.z(), -point_ov.scalar_product(vector_i)},
			{vector_j.x(), vector_j.y(), vector_j.z(), -point_ov.scalar_product(vector_j)},
			{vector_k.x(), vector_k.y(), vector_k.z(), -point_ov.scalar_product(vector_k)},
			{0, 0, 0, 1}
		});

		input_matrix = affine_matrix * input_matrix;

		return input_matrix;
	}

	static matrix<double> convert_view_to_projection(
		const bool is_ort,
		const double f_value,
		matrix<double>& input_matrix
	)
	{
		matrix<double> affine_matrix(1, 1);
		if (is_ort)
		{
			affine_matrix = matrix<double>(std::vector<std::vector<double>>{
				{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 0, 1}
				});
		} else
		{
			affine_matrix = matrix<double>(std::vector<std::vector<double>>{
				{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, -1 / f_value, 1}
				});
		}
		
		return affine_matrix * input_matrix;
	}

public:
	static matrix<double> convert_world_to_screen(
		matrix<double> input_matrix,
		const vector_3d& vector_i,
		const vector_3d& vector_j,
		const vector_3d& vector_k,
		const point_double_3d point_ov,
		const bool is_ort,
		const double f_value,
		const point_double point_world0_in_screen,
		const point_double pixel
	)
	{

		const auto world_to_view = new matrix<double>(std::vector<std::vector<double>>{
			{vector_i.x(), vector_j.x(), vector_k.x(), 0},
			{vector_i.y(), vector_j.y(), vector_k.y(), 0},
			{vector_i.z(), vector_j.z(), vector_k.z(), 0},
			{-point_ov.scalar_product(vector_i), -point_ov.scalar_product(vector_j), -point_ov.scalar_product(vector_k), 1}
		});


		matrix<double>* view_to_projection;
		if (/*f_value >= 1000*/true)
		{
			view_to_projection = new matrix<double>(std::vector<std::vector<double>>{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 0},
				{0, 0, 1}
			});
		}
		else
		{
			
			view_to_projection = new matrix<double>(std::vector<std::vector<double>>{
				{1, 0, 0},
				{ 0, 1, 0 },
				{ 0, 0, -1 / f_value },
				{ 0, 0, 1 }
			});
		}

		const auto projection_to_screen = new matrix<double>(std::vector<std::vector<double>>{
			{1, 0, 0},
			{0, 1, 0},
			{point_world0_in_screen.x / pixel.x, -point_world0_in_screen.y / pixel.y, 1}
		});

		std::fstream out("output.txt", std::ios::app);

		out << "---------------------------------------------------\n";
		logger::log(out, input_matrix, "input_matrix");
		logger::log(out, *world_to_view, "w->v");
		logger::log(out, *view_to_projection, "v->p");
		logger::log(out, *projection_to_screen, "p->s");

		auto result = input_matrix * *world_to_view;
		logger::log(out, result, "inputw-inputv");
		result = result * *view_to_projection;
		logger::log(out, result , "inputv-inputp");
		result = result * *projection_to_screen;
		logger::log(out, result, "inputp-inputs");

		out.close();
		return result;
			/*input_matrix
		*
			*world_to_view
		* 
			*view_to_projection
		* 
			*projection_to_screen;*/
	}

	/*static matrix<double> convert_world_to_projection(
		matrix<double>& input_matrix,
		const vector_3d& vector_i,
		const vector_3d& vector_j,
		const vector_3d& vector_k,
		const point_double_3d& point_ov,
		const bool is_ort,
		const double f_value
	)
	{

		

		return	input_matrix * *world_to_view * *view_to_projection;

	}*/

};
#endif