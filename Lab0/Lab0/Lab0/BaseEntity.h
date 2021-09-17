#pragma once
#include "VideoEngineObject.h"
class BaseEntity :
	public VideoEngineObject
{
private:
	double* coordinate_x_;
	double* coordinate_y_;
	int* size_x_;
	int* size_y_;

public:
	//Constructors
	BaseEntity() :
		coordinate_x_(new double(0)),
		coordinate_y_(new double(0)),
		size_x_(new int(0)),
		size_y_(new int(0))
	{
	}

	BaseEntity(
		const double& value_coordinate_x,
		const double& value_coordinate_y,
		const int& value_size_x,
		const int& value_size_y
	) : 
		coordinate_x_(new double(value_coordinate_x)),
		coordinate_y_(new double(value_coordinate_y)),
		size_x_(new int(value_size_x)),
		size_y_(new int(value_size_y))
	{
	}
	
	//getters
	double get_coordinate_x() const
	{
		return *coordinate_x_;
	}

	double get_coordinate_y() const
	{
		return *coordinate_y_;
	}

	int get_size_x() const
	{
		return *size_x_;
	}

	int get_size_y() const
	{
		return *size_y_;
	}

	//setters
	void set_coordinate_x(
		double& value
	)
	{
		this->coordinate_x_ = &value;
	}

	void set_coordinate_y(
		double& value
	)
	{
		this->coordinate_y_ = &value;
	}

	void set_size_x(
		int& value
	)
	{
		this->size_x_ = &value;
	}

	void set_size_y(
		int& value
	)
	{
		this->size_y_ = &value;
	}
};
