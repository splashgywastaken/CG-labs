#ifndef GRAPH_2D
#define GRAPH_2D

#include "camera_2d.h"
#include "graph_types.h"
#include "PointDouble.h"

class graph_2d
{
	//Описание графика
	//Фактически я разделяю работу с х и у на две разные части и дальше провожу вычисления именно для них
	//А не для одной функции
	//Работаем с полярными координатами
	graph_types graph_type_;//Описывается в виде одного из элементов перечисления
	//Параметры для x и y при параметрическом задании функции
	point_double params_;

	//Координаты начального положения графика функции в мировых координатах
	POINT position_;

	POINT delta_{};

	//Предыдущие координаты графика (нужно для перемещения графика мышкой)
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

	//Функция берет на вооружение переменные, которые нужны для задания размера по отношению к окну
	//И использует эти переменные в расчетах. Можно будет сделать расчеты тут,
	//либо выделить отдельный класс со статическими функциями
	void draw(HDC hdc, point_double point_world0, point_double pixel, double L, double R) const;

	void move(POINT new_point);

};
#endif