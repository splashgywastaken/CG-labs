#ifndef POINT_DOUBLE_3D_H
#define POINT_DOUBLE_3D_H
#include "vector_3d.h"

class point_double_3d
{

	double x_;
	double y_;
	double z_;

public:

	void set(
		const double x, 
		const double y, 
		const double z
	)
	{
		x_ = x;
		y_ = y;
		z_ = z;
	}

	double scalar_product(const vector_3d& vector) const
	{
		return
			x_ * vector.x() + y_ * vector.y() + z_ * vector.z();
	}

	::point_double_3d& operator-=(const point_double_3d& vector);
	::point_double_3d& operator+=(const point_double_3d& vector);
	::point_double_3d& operator-();

	double& operator[](const size_t i){

		if (i == 0)
		{
			return x_;
		}
		if (i == 1)
		{
			return y_;
		}

		return z_;

	}

	point_double_3d operator+(const point_double_3d& position) const
	{

		return
			point_double_3d{x_ + position.x_, y_ + position.y_, z_ + position.z_};

	}

	point_double_3d() : x_(0.0), y_(0.0), z_(0.0)
	{
	}
	point_double_3d(
		const double& x,
		const double& y,
		const double& z
	)
	: x_(x), y_(y), z_(z) {}
	point_double_3d(const point_double_3d& other) = default;
	point_double_3d(point_double_3d&& other) noexcept
		: x_(other.x_),
		  y_(other.y_),
		  z_(other.z_)
	{
	}

	~point_double_3d() = default;

	point_double_3d& operator=(const point_double_3d& other)
	{
		if (this == &other)
			return *this;
		x_ = other.x_;
		y_ = other.y_;
		z_ = other.z_;
		return *this;
	}
	point_double_3d& operator=(point_double_3d&& other) noexcept
	{
		if (this == &other)
			return *this;
		x_ = other.x_;
		y_ = other.y_;
		z_ = other.z_;
		return *this;
	}

	friend bool operator==(const point_double_3d& lhs, const point_double_3d& rhs)
	{
		return lhs.x_ == rhs.x_
			&& lhs.y_ == rhs.y_
			&& lhs.z_ == rhs.z_;
	}
	friend bool operator!=(const point_double_3d& lhs, const point_double_3d& rhs)
	{
		return !(lhs == rhs);
	}

	friend bool operator<(const point_double_3d& lhs, const point_double_3d& rhs)
	{
		if (lhs.x_ < rhs.x_)
			return true;
		if (rhs.x_ < lhs.x_)
			return false;
		if (lhs.y_ < rhs.y_)
			return true;
		if (rhs.y_ < lhs.y_)
			return false;
		return lhs.z_ < rhs.z_;
	}
	friend bool operator<=(const point_double_3d& lhs, const point_double_3d& rhs)
	{
		return !(rhs < lhs);
	}
	friend bool operator>(const point_double_3d& lhs, const point_double_3d& rhs)
	{
		return rhs < lhs;
	}
	friend bool operator>=(const point_double_3d& lhs, const point_double_3d& rhs)
	{
		return !(lhs < rhs);
	}

	double x() const
	{
		return this->x_;
	}
	double y() const
	{
		return this->y_;
	}
	double z() const
	{
		return this->z_;
	}
};
inline point_double_3d& point_double_3d::operator+=(const point_double_3d& vector)
{

	this->x_ += vector.x();
	this->y_ += vector.x();
	this->z_ += vector.x();

	return *this;

}

inline point_double_3d& point_double_3d::operator-()
{

	this->x_ *= -1;
	this->y_ *= -1;
	this->z_ *= -1;

	return *this;

}

inline point_double_3d& point_double_3d::operator-=(const point_double_3d& vector)
{

	this->x_ -= vector.x();
	this->y_ -= vector.x();
	this->z_ -= vector.x();

	return *this;

}
#endif
