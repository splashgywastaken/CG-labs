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
		} else 
		if (rotation[1] != 0.0)
		{
			input_matrix = rotate(input_matrix, rotation_type_3d::ordinate, rotation[1]);
		} else
			input_matrix = rotate(input_matrix, rotation_type_3d::applicate, rotation[2]);
		
		input_matrix = move(input_matrix, -move_point);

		return input_matrix;

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

	//�������� �������������� ��� ���������� �������:

	//���������� �������������� (��� ������������������ ����)
	//������� �� ������ �(x,y,z)
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
	//������� ������ ���� (�������, �������, ��������) �� ���� ��
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
	//��������������� ����� ������������ ����
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
	//��������� ������������ ���� � ����������
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