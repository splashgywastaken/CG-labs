#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H
#include "mirror_type_3d.h"
#include "Matrix.h"
#include "PointDouble.h"
#include "point_double_3d.h"
#include "rotation_type_3d.h"

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

	static matrix<double> custom_rotation(
		double radian,
		point_double centre_point,
		matrix<double>& input_matrix
	)
	{
		return matrix<double>(1, 2);
	}

	static matrix<double>& custom_scaling(
		point_double scale,
		double rotation_radian,
		matrix<double> previous_point,
		point_double centre_point,
		matrix<double>& input_matrix
	)
	{
		
	}

	//Аффинные преобразования для трехмерных моделей:

	//Переписать преобразования (для транспонированного вида)
	//Перенос на вектор а(x,y,z)
	static matrix<double> move(matrix<double>& input_matrix, const point_double_3d& vector)
	{
		const auto move_matrix = new matrix<double>(std::vector<std::vector<double>>
			({
				{1, 0, 0, vector.x()},
				{0, 1, 0, vector.y()},
				{0, 0, 1, vector.z()},
				{0, 0, 0, 1}
				})
			);

		return
			*move_matrix * input_matrix;

	}
	//Поворот вокруг осей (абсцисс, ординат, аппликат) на угол фи
	static matrix<double> rotate(
		matrix<double>& input_matrix,
		const rotation_type_3d type,
		const double phi
	)
	{

		matrix<double> *rotation_matrix;

		switch (type)
		{
			case rotation_type_3d::abscissa:
				rotation_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{1,        0,         0, 0},
						{0, cos(phi), -sin(phi), 0},
						{0, sin(phi),  cos(phi), 0},
						{0,        0,         0, 1}
						})
				);
				break;

			case rotation_type_3d::applicate:
				rotation_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{ cos(phi), -sin(phi), 0, 0},
						{ sin(phi),  cos(phi), 0, 0},
						{        0,         0, 0, 0},
						{        0,         0, 0, 1}
						})
				);
				break;

			case rotation_type_3d::ordinate:
				rotation_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{ cos(phi), 0, sin(phi), 0},
						{        0, 1,        0, 0},
						{-sin(phi), 0, cos(phi), 0},
						{        0, 0,        0, 1}
						})
				);
				break;

			default:
				rotation_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{1, 0, 0, 0},
						{0, 1, 0, 0},
						{0, 0, 1, 0},
						{0, 0, 0, 1}
						})
				);
				break;
		}

		return
			*rotation_matrix * input_matrix;

	}
	//Масштабирование вдоль координатных осей
	static matrix<double> scale(matrix<double>& input_matrix, const point_double_3d& scale_params)
	{
		const auto rotation_matrix = new matrix<double>(std::vector<std::vector<double>>
			({
				{scale_params.x(),                0,                0, 0},
				{               0, scale_params.y(),                0, 0},
				{               0,                0, scale_params.z(), 0},
				{               0,                0,                0, 1}
				})
			);

		return
			*rotation_matrix * input_matrix;

	}
	//Отражение относительно осей и плоскостей
	static matrix<double> mirror(matrix<double>& input_matrix, const mirror_type_3d& type)
	{
		matrix<double>* mirror_matrix;

		switch(type)
		{
			case mirror_type_3d::yz:
				mirror_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{-1, 0, 0, 0},
						{ 0, 1, 0, 0},
						{ 0, 0, 1, 0},
						{ 0, 0, 0, 1}
						})
					);
				break;

			case mirror_type_3d::zx:
				mirror_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{1,  0, 0, 0},
						{0, -1, 0, 0},
						{0,  0, 1, 0},
						{0,  0, 0, 1}
						})
					);
				break;

			case mirror_type_3d::xy:
				mirror_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{1, 0,  0, 0},
						{0, 1,  0, 0},
						{0, 0, -1, 0},
						{0, 0,  0, 1}
						})
					);
				break;

			case mirror_type_3d::abscissa:
				mirror_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{1,  0,  0, 0},
						{0, -1,  0, 0},
						{0,  0, -1, 0},
						{0,  0,  0, 1}
						})
					);
				break;

			case mirror_type_3d::ordinate:
				mirror_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{-1, 0, 0, 0},
						{ 0, 1, 0, 0},
						{ 0, 0, -1, 0},
						{ 0, 0,  0, 1}
						})
					);
				break;
				
			case mirror_type_3d::applicate:
				mirror_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{-1,  0, 0, 0},
						{ 0, -1, 0, 0},
						{ 0,  0, 1, 0},
						{ 0,  0, 0, 1}
						})
					);
				break;

			case mirror_type_3d::zero:
				mirror_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{-1,  0,  0, 0},
						{ 0, -1,  0, 0},
						{ 0,  0, -1, 0},
						{ 0,  0,  0, 1}
						})
					);
				break;

			default:
				mirror_matrix = new matrix<double>(std::vector<std::vector<double>>
					({
						{1, 0, 0, 0},
						{0, 1, 0, 0},
						{0, 0, 1, 0},
						{0, 0, 0, 1}
						})
					);
					break;
		}

		return
			*mirror_matrix * input_matrix;

	}
};
#endif