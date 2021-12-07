#pragma once
#include "framework.h"

struct point_double
{

	double x,
		   y;

	point_double() { x = 0, y = 0; }

	explicit point_double(const POINT& point)
	{
		x = static_cast<double>(point.x);
		y = static_cast<double>(point.y);
	}

	point_double(const int x, const int y)
	{

		this->x = x;
		this->y = y;

	}

	point_double(const double x, const double y)
	{

		this->x = x;
		this->y = y;

	}

	::point_double operator*(double& value) const
	{
		return
		{
			this->x * -1,
			this->y * -1
		};
	}

	point_double operator*(double x) const;
};

inline point_double point_double::operator*(double x) const
{
	return
	{
		this->x * x,
		this->y * x
	};
}
