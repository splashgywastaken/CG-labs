#ifndef GRAPH_2D
#define GRAPH_2D

#include "camera_2d.h"
#include "graph_types.h"

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

	//���������� ���������� ��������� ������� �������
	int pos_x,
		pos_y;
	//���������� ���������� ������� (����� ��� ����������� ������� ������)
	int lastpos_x,
		lastpos_y;

public:
	graph_2d(int pos_x, int pos_y)
		: pos_x(pos_x),
		  pos_y(pos_y)
	{

		lastpos_x = pos_x;
		lastpos_y = pos_y;
	}

	graph_2d(const graph_2d& other)
		: graph_type(other.graph_type),
		  x_param(other.x_param),
		  y_param(other.y_param),
		  pos_x(other.pos_x),
		  pos_y(other.pos_y)
	{
		lastpos_x = pos_x;
		lastpos_y = pos_y;
	}

	graph_2d(graph_2d&& other) noexcept
		: graph_type(std::move(other.graph_type)),
		  x_param(other.x_param),
		  y_param(other.y_param),
		  pos_x(other.pos_x),
		  pos_y(other.pos_y)
	{
		lastpos_x = pos_x;
		lastpos_y = pos_y;
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
		lastpos_x = other.lastpos_x;
		lastpos_y = other.lastpos_y;
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
		return *this;
	}

	friend bool operator==(const graph_2d& lhs, const graph_2d& rhs)
	{
		return lhs.graph_type == rhs.graph_type
			&& lhs.x_param == rhs.x_param
			&& lhs.y_param == rhs.y_param
			&& lhs.pos_x == rhs.pos_x
			&& lhs.pos_y == rhs.pos_y
			&& lhs.lastpos_x == rhs.lastpos_x
			&& lhs.lastpos_y == rhs.lastpos_y;
	}

	friend bool operator!=(const graph_2d& lhs, const graph_2d& rhs)
	{
		return !(lhs == rhs);
	}

	void set_graph_type(std::string str, double x_param, double y_param);

	//������� ����� �� ���������� ����������, ������� ����� ��� ������� ������� �� ��������� � ����
	//� ���������� ��� ���������� � ��������. ����� ����� ������� ������� ���,
	//���� �������� ��������� ����� �� ������������ ���������
	void draw(HDC hdc, int W, int H, double L, double R, double T, double B, double px_x, double px_y);

};
#endif