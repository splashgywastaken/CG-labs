#pragma once
#include "framework.h"

struct PointDouble
{

	double x,
		   y;

	PointDouble() { x = 0, y = 0; }

	PointDouble(const POINT& point)
	{
		x = static_cast<double>(point.x);
		y = static_cast<double>(point.y);
	}

	PointDouble(int _x, int _y)
	{

		x = _x;
		y = _y;

	}
};