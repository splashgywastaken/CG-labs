#ifndef GRAPH_2D
#define GRAPH_2D

#include "camera_2d.h"
#include "graph_types.h"

class graph_2d
{
	//Описание графика
	//Фактически я разделяю работу с х и у на две разные части и дальше провожу вычисления именно для них
	//А не для одной функции
	//Работаем с полярными координатами
	graph_types graph_type;//Описывается в виде одного из элементов перечисления
	//Параметры для x и y при параметрическом задании функции
	double x_param,
	       y_param;

	//Координаты начального положения графика функции
	int pos_x,
		pos_y;
	//Предыдущие координаты графика (нужно для перемещения графика мышкой)
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

	//Функция берет на вооружение переменные, которые нужны для задания размера по отношению к окну
	//И использует эти переменные в расчетах. Можно будет сделать расчеты тут,
	//либо выделить отдельный класс со статическими функциями
	void draw(HDC hdc, int W, int H, double L, double R, double T, double B, double px_x, double px_y);

};
#endif