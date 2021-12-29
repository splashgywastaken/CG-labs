#pragma once

struct point_double
{

	double x,
		   y;

	point_double() { x = 0, y = 0; }
	point_double(const double x, const double y) : x(x), y(y)
	{
	}
	~point_double() = default;

	point_double(const point_double& other) = default;

	point_double(point_double&& other) noexcept
		: x(other.x),
		  y(other.y)
	{
	}

	point_double& operator=(const point_double& other)
	{
		if (this == &other)
			return *this;
		x = other.x;
		y = other.y;
		return *this;
	}

	point_double& operator=(point_double&& other) noexcept
	{
		if (this == &other)
			return *this;
		x = other.x;
		y = other.y;
		return *this;
	}

	explicit point_double(const POINT& point) : x(static_cast<double>(point.x)), y(static_cast<double>(point.y))
	{
	}

	point_double(const int x, const int y) : x(x), y(y)
	{
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
