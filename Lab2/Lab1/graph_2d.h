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
	graph_types graph_type;//����������� � ���� ������ �� ��������� ������������
	//��������� ��� x � y ��� ��������������� ������� �������
	double x_param,
	       y_param;

	//���������� ���������� ��������� ������� ������� � ������� �����������
	int pos_x,
		pos_y;

	int delta_x,
		delta_y;

	//���������� ���������� ������� (����� ��� ����������� ������� ������)
	POINT lastpos;

public:
	graph_2d(int pos_x, int pos_y)
		: pos_x(pos_x),
		  pos_y(pos_y)
	{
		lastpos.x = pos_x;
		lastpos.y = pos_y;
		delta_x = 0;
		delta_y = 0;
	}

	graph_2d(const graph_2d& other)
		: graph_type(other.graph_type),
		  x_param(other.x_param),
		  y_param(other.y_param),
		  pos_x(other.pos_x),
		  pos_y(other.pos_y)
	{
		lastpos.x = pos_x;
		lastpos.y = pos_y;
	}

	graph_2d(graph_2d&& other) noexcept
		: graph_type(std::move(other.graph_type)),
		  x_param(other.x_param),
		  y_param(other.y_param),
		  pos_x(other.pos_x),
		  pos_y(other.pos_y)
	{
		lastpos.x = pos_x;
		lastpos.y = pos_y;
	}

	graph_2d& operator=(const graph_2d& other)
	{
		if (this == &other)
			return *this;
		graph_type = other.graph_type;
		x_param = other.x_param;
		y_param = other.y_param;
		pos_x = other.pos_x;
		pos_y = other.pos_y;
		lastpos.x = other.lastpos.x;
		lastpos.y = other.lastpos.y;
		return *this;
	}

	graph_2d& operator=(graph_2d&& other) noexcept
	{
		if (this == &other)
			return *this;
		graph_type = std::move(other.graph_type);
		x_param = other.x_param;
		y_param = other.y_param;
		pos_x = other.pos_x;
		pos_y = other.pos_y;
		lastpos.x = other.lastpos.x;
		lastpos.y = other.lastpos.y;
		return *this;
	}

	friend bool operator==(const graph_2d& lhs, const graph_2d& rhs)
	{
		return lhs.graph_type == rhs.graph_type
			&& lhs.x_param == rhs.x_param
			&& lhs.y_param == rhs.y_param
			&& lhs.pos_x == rhs.pos_x
			&& lhs.pos_y == rhs.pos_y
			&& lhs.lastpos.x == rhs.lastpos.x
			&& lhs.lastpos.y == rhs.lastpos.y;
	}

	friend bool operator!=(const graph_2d& lhs, const graph_2d& rhs)
	{
		return !(lhs == rhs);
	}

	void set_graph_type(std::string data, double x_param, double y_param);

	//������� ����� �� ���������� ����������, ������� ����� ��� ������� ������� �� ��������� � ����
	//� ���������� ��� ���������� � ��������. ����� ����� ������� ������� ���,
	//���� �������� ��������� ����� �� ������������ ���������
	void draw(HDC hdc, POINT point_world0, point_double pixel, double L, double R) const;

	void move(POINT new_point);

};
#endif