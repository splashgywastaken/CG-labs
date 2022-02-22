#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H
#include "mirror_type_3d.h"
#include "Matrix.h"
#include "PointDouble.h"
#include "point_double_3d.h"
#include "rotation_type_3d.h"
#include "Logger.h"
#include <fstream>

constexpr double PI = 3.14159265;

//Matrix<> Translation(double x, double y)
//{
//	double T[9] = {
//		1, 0, x,
//		0, 1, y,
//		0, 0, 1};
//	return Matrix<>(3, T);
//}
//
// ����� ��������� ����������� �������, ������������ ������� ������� ��:
// Identity() - ������������� ��;
// Rotation(t) - ������� �� ���� t;
// Rotation(c, s) - ������� �� ����, ������� � ����� �������� ��������������� ��������� c � s;
// Scaling(kx, ky) - ���������������;
// Mapping (��������� ���� ���������) - �� �������, ��� �������� ������ ��������� ����� ������������ ������� Scaling.
//
// � ���������� ������� ������������ ����������� ������� �� ������.
// �������������� ���� � ������ �������� �������� ������.
// ��������, ����� �������, ��������� ��������� �������,
// � ����������� ���������������� ��������� � �������������.

class affine_transform
{
public:

	static matrix<double> custom_rotation(
		point_double_3d rotation,
		point_double_3d move_point,
		matrix<double>& input_matrix
	)
	{

		input_matrix = move(input_matrix, -move_point);

		if (rotation[0] != 0.0)
		{
			input_matrix = rotate(input_matrix, rotation_type_3d::abscissa, rotation[0]);
		}
		if (rotation[1] != 0.0)
		{
			input_matrix = rotate(input_matrix, rotation_type_3d::ordinate, rotation[1]);
		}
		if (rotation[2] != 0.0)
			input_matrix = rotate(input_matrix, rotation_type_3d::applicate, rotation[2]);
		
		input_matrix = move(input_matrix, -move_point);

		return input_matrix;

	}

	static matrix<double> rotation_by_edge(
		matrix<double>& input_matrix,
		const point_double_3d& init_rotation,
		const double& rotation_radians,
		int first_edge_vertex,
		int second_edge_vertex
	)
	{
		///
		/// 1 - переместить модель используя первую вершину ребра,
		///	2 - повернуть её так, чтобы координаты ребра совпали с одной из осей
		///	3 - второй пункт, но для другой оси
		///	4 - основной поворот вокруг третьей оси
		///	5 - обратный к 3
		///	6 - обратный к 2
		///	7 - обратный к 1
		///
		///	углы для 2 и 3 искать через отношения сторон (координат точек)
		///
		///	проверить по отдельности как работает каждый этап
		///

		first_edge_vertex -= 1;
		second_edge_vertex -= 1;

		//Перемещение модели в центр координат
		auto k_1 = new double(1 / input_matrix[3][first_edge_vertex]);
		auto k_2 = new double(1 / input_matrix[3][second_edge_vertex]);

		point_double_3d* move_vector = new point_double_3d(
			{
				(input_matrix[0][first_edge_vertex] * *k_1 + input_matrix[0][second_edge_vertex] * *k_2) / 2,
				(input_matrix[1][first_edge_vertex] * *k_1 + input_matrix[1][second_edge_vertex] * *k_2) / 2,
				(input_matrix[2][first_edge_vertex] * *k_1 + input_matrix[2][second_edge_vertex] * *k_2) / 2
			}
		);

		//Перемещение модели в центр координат
		input_matrix = affine_transform::move(input_matrix, -*move_vector);

		//Поворот отнистельно оси аппликат для "восстановления положения" модели относительно данной оси
		input_matrix = affine_transform::rotate(input_matrix, rotation_type_3d::applicate, -init_rotation.z());

		//Поворот отнистельно оси ординат для "восстановления положения" модели относительно данной оси
		input_matrix = affine_transform::rotate(input_matrix, rotation_type_3d::ordinate, -init_rotation.y());

		//поворот вокруг оси 
		input_matrix = affine_transform::rotate(input_matrix, rotation_type_3d::abscissa, rotation_radians);

		//Обратный поворот к повороту относительно оси ординат
		input_matrix = affine_transform::rotate(input_matrix, rotation_type_3d::ordinate, init_rotation.y());

		//Обратный поворот к повороту относительно оси аппликат
		input_matrix = affine_transform::rotate(input_matrix, rotation_type_3d::applicate, init_rotation.z());

		//Перемещение модели, обратное к перемещению в центр координат
		input_matrix = affine_transform::move(input_matrix, -*move_vector);

		delete move_vector;
		delete k_1;
		delete k_2;

		return input_matrix;

	}
	
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

		input_matrix = *move_matrix * input_matrix;

		delete move_matrix;

		return input_matrix;
	}

	static matrix<double> rotate(
		matrix<double>& input_matrix,
		const rotation_type_3d type,
		double phi
	)
	{

		phi *= 3.14159265 / 180;

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
						{        0,         0, 1, 0},
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

		input_matrix = *rotation_matrix * input_matrix;

		delete rotation_matrix;

		return input_matrix;

	}

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

		input_matrix = *rotation_matrix * input_matrix;

		delete rotation_matrix;

		return input_matrix;

	}

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

		input_matrix = *mirror_matrix * input_matrix;

		delete mirror_matrix;

		return input_matrix;
	}
};
#endif