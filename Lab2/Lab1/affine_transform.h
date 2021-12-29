#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H
#include "Matrix.h"
#include "PointDouble.h"

//Matrix<> Translation(double x, double y)
//{
//	double T[9] = {
//		1, 0, x,
//		0, 1, y,
//		0, 0, 1};
//	return Matrix<>(3, T);
//}
//
// Здесь требуется реализовать функции, возвращающие матрицы базовых АП:
// Identity() - тождественное АП;
// Rotation(t) - поворот на угол t;
// Rotation(c, s) - поворот на угол, косинус и синус которого пропорциональны величинам c и s;
// Scaling(kx, ky) - масштабирование;
// Mapping (различные виды отражений) - по желанию, для создания матриц отражения можно использовать функцию Scaling.
//
// В приведённом примере используется конструктор матрицы из списка.
// Приветствуются идеи о других способах создания матриц.
// Например, вызов функции, создающей единичную матрицу,
// с последующим переопределением некоторых её коэффициентов.

class affine_transform
{
public:
	static matrix<double>& parallel_move_by_vector(point_double vector, matrix<double>& input_matrix)
	{

		matrix<double> move_matrix(std::vector<std::vector<double>>{
			{ 1.0, 0.0, vector.x},
			{ 0.0, 1.0, vector.y},
			{ 0.0, 0.0, 1.0}
		});

		return move_matrix * input_matrix;

	}

	/*static matrix<double> rotation(double rad, matrix<double> input_matrix)
	{

		matrix<double> rotation_matrix(std::vector<std::vector<double>>{
			{cos(rad), -sin(rad), 0},
			{sin(rad),  cos(rad), 0},
			{       0,         0, 1}
		});

		return rotation_matrix * input_matrix;

	}

	static matrix<double> rotation(double cos, double sin, matrix<double>& input_matrix)
	{

		matrix<double> rotation_matrix(std::vector<std::vector<double>>{
			{cos, -sin, 0},
			{sin,  cos, 0},
			{0,      0, 1}
		});

		return rotation_matrix * input_matrix;

	}*/

	static matrix<double>& rotation(POINT rotation_type, matrix<double>& input_matrix)
	{

		matrix<double> rotation_matrix(std::vector<std::vector<double>>{
			{static_cast<double>(rotation_type.x), 0, 0},
			{ 0, static_cast<double>(rotation_type.y), 0 },
			{ 0, 0, 1 }
		});

		return rotation_matrix * input_matrix;

	}

	static matrix<double>& rotation(double radian, matrix<double>& input_matrix)
	{

		matrix<double> rotation_matrix(std::vector<std::vector<double>>{
			{cos(radian), -sin(radian), 0},
			{ sin(radian), cos(radian), 0},
			{ 0, 0, 1}
		});

		return rotation_matrix * input_matrix;

	}

	static  matrix<double> rotation(double cos, double sin, matrix<double>& input_matrix)
	{
		if (cos == 0.0 && sin == 0.0)
			return input_matrix;
		matrix<double> rotation_matrix(std::vector<std::vector<double>>{
			{cos, -sin, 0},
			{sin,  cos, 0},
			{0,      0, 1}
		});

		return rotation_matrix * input_matrix;

	}

	static matrix<double> scaling(point_double k, matrix<double> input_matrix)
	{

		matrix<double> scale_matrix (std::vector<std::vector<double>>{
			{k.x,  0, 0},
			{ 0, k.y, 0},
			{ 0,  0, 1}
		});

		return scale_matrix * input_matrix;
	}
	//
	//Доп задания
	//1. Поворот вокруг центра пакмана
	//   создать функцию rotation (которая принимает угол иск и игрек), а в этой функции перемножаешь
	//   три матрицы: переноса, поворота, обратного переноса
	//2. Масштабирование в доль осей самого пакмана (чтобы при уменьшении оставался в той-же точке)
	//   Создать скейлинг, принимающий kx ky, а также x0, y0 - координаты точки, которая ост. неизм.
	//   при масштабировании
	//   c и s  
	//

	static matrix<double> custom_rotation(
		double radian,
		matrix<double> previous_point,
		point_double centre_point,
		matrix<double>& input_matrix
	)
	{
		double k = 1 / previous_point[0][2];
		const point_double move_coordinates = 
		{ previous_point[0][0] * k + centre_point.x, previous_point[0][1] * k + centre_point.y};

		input_matrix = parallel_move_by_vector(move_coordinates * -1, input_matrix);
		//input_matrix = parallel_move_by_vector(centre_point * -1, input_matrix);
		input_matrix = rotation(radian, input_matrix);
		input_matrix = parallel_move_by_vector(move_coordinates, input_matrix);
		//input_matrix = parallel_move_by_vector(centre_point, input_matrix);
		

		return input_matrix;

	}

	static matrix<double> custom_scaling(
		const point_double scale,
		const double rotation_radian,
		matrix<double> previous_point,
		const point_double centre_point,
		matrix<double>& input_matrix
	)
	{
		const double k = 1 / previous_point[0][2];
		const point_double move_coordinates =
		{ previous_point[0][0] * k + centre_point.x, previous_point[0][1] * k + centre_point.y };

		input_matrix = parallel_move_by_vector(move_coordinates * -1, input_matrix);
		input_matrix = rotation(-rotation_radian, input_matrix);
		input_matrix = scaling(scale, input_matrix);
		input_matrix = rotation(rotation_radian, input_matrix);
		input_matrix = parallel_move_by_vector(move_coordinates, input_matrix);

		return input_matrix;
	}
};
#endif