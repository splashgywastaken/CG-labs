#ifndef GRAPH_2D
#define GRAPH_2D

#include "camera_2d.h"
#include "graph_types.h"
#include "PointDouble.h"

class graph_2d
{
	//�������� �������
	//���������� � �������� ������ � � � � �� ��� ������ ����� � ������ ������� ���������� ������ ��� ���
	//� �� ��� ����� �������
	//�������� � ��������� ������������
	graph_types graph_type_;//����������� � ���� ������ �� ��������� ������������
	//��������� ��� x � y ��� ��������������� ������� �������
	point_double params_;

	//���������� ���������� ��������� ������� ������� � ������� �����������
	POINT position_;

	POINT delta_{};

	//���������� ���������� ������� (����� ��� ����������� ������� ������)
	POINT last_position_{};

public:
	graph_2d(const int pos_x, const int pos_y)
		: graph_type_(), position_{pos_x, pos_y}
	{
		last_position_.x = pos_x;
		last_position_.y = pos_y;
		delta_.x = 0;
		delta_.y = 0;
	}

	friend bool operator==(const graph_2d& lhs, const graph_2d& rhs)
	{
		return lhs.graph_type_ == rhs.graph_type_
			&& (lhs.params_.x == rhs.params_.x && lhs.params_.y == rhs.params_.y)
			&& (lhs.position_.x == rhs.position_.x && lhs.position_.y == rhs.position_.y)
			&& (lhs.delta_.x == rhs.delta_.x && lhs.delta_.y == rhs.delta_.y)
			&& (lhs.last_position_.x == rhs.last_position_.x && lhs.last_position_.y == rhs.last_position_.y);
	}

	friend bool operator!=(const graph_2d& lhs, const graph_2d& rhs)
	{
		return !(lhs == rhs);
	}

	void set_graph_type(std::string data, point_double params);

	//������� ����� �� ���������� ����������, ������� ����� ��� ������� ������� �� ��������� � ����
	//� ���������� ��� ���������� � ��������. ����� ����� ������� ������� ���,
	//���� �������� ��������� ����� �� ������������ ���������
	void draw(HDC hdc, point_double point_world0, point_double pixel, double L, double R) const;

	void move(POINT new_point);

};
#endif