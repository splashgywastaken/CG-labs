#ifndef VECTOR_3D_H
#define VECTOR_3D_H
#include <cmath>

class vector_3d
{

	double* x_;
	double* y_;
	double* z_;

public:
	vector_3d(
		const double& x, 
		const double& y,
		const double& z
	)
	:	x_(new double(x)),
		y_(new double(y)),
		z_(new double(z))
	{
	}

	~vector_3d()
	{
		delete x_;
		delete y_;
		delete z_;
	}

	//Copy and move operators
	vector_3d(const vector_3d& other)
	:		x_(new double(other.x())),
			y_(new double(other.y())),
			z_(new double(other.z()))
	{
	}

	vector_3d(vector_3d&& other) noexcept
	:		x_(new double(other.x())),
			y_(new double(other.y())),
			z_(new double(other.z()))
	{
	}

	vector_3d& operator=(const vector_3d& other)
	{
		if (this == &other)
			return *this;
		*x_ = other.x();
		*y_ = other.y();
		*z_ = other.z();
		return *this;
	}

	vector_3d& operator=(vector_3d&& other) noexcept
	{
		if (this == &other)
			return *this;
		*x_ = other.x();
		*y_ = other.y();
		*z_ = other.z();
		return *this;
	}

	vector_3d operator+(const vector_3d& other) const
	{
		vector_3d result = {
			this->x() + other.x(),
			this->y() + other.y(),
			this->z() + other.z()
		};

		return result;
	}

	vector_3d operator*(const double k) const
	{
		vector_3d result = {
			this->x() * k,
			this->y() * k,
			this->z() * k
		};

		return result;
	}

	vector_3d operator/(double k) const
	{
		k = 1 / k;

		return *this * k;
	}

	double operator[](int i)
	{
		if (i == 0)
			return x();
		else
			if (i == 1) {
				return y();
			}
			else
				if (i == 2)
					return z();
	}

	//Векторное произведение
	vector_3d vector_product(const vector_3d& other) const
	{
		vector_3d result =
		{
			this->y() * other.z() - this->z() * other.y(),
			-(this->x() * other.z() - this->z() * other.x()),
			this->x() * other.y() - this->y() * other.x()
		};
		return result;
	}
	//Скалярное произведение
	double scalar_product(const vector_3d& other) const
	{
		return
			this->x() * other.x() + this->y() * other.y() + this->z() * other.z();
	}

	double norm() const
	{
		return
			sqrt(
				pow(
				this->x(), 2) 
				+
				pow(this->y(), 2) 
				+
				pow(this->z(), 2)
			);
	}

	//Getters and setters
	//x
	double x() const
	{
		return *x_;
	}
	void set_x(const double& x) const
	{
		*x_ = x;
	}
	//y
	double y() const
	{
		return *y_;
	}
	void set_y(const double& y) const 
	{
		*y_ = y;
	}
	//z
	double z() const
	{
		return *z_;
	}
	void set_z(const double& z) const
	{
		*z_ = z;
	}
	//set all
	void set(
		const double& x,
		const double& y,
		const double& z
	) const
	{
		*x_ = x;
		*y_ = y;
		*z_ = z;
	}
};
#endif