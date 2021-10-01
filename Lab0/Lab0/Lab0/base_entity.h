#pragma once
#include "video_engine_object.h"
class base_entity :
	virtual public video_engine_object
{
private:
	int coordinate_x_;
	int coordinate_y_;
	int size_x_;
	int size_y_;

public:
	//Constructors
	base_entity() :
		coordinate_x_(0),
		coordinate_y_(0),
		size_x_(0),
		size_y_(0)
	{
		coordinate_x_ = 0;
		coordinate_y_ = 0;
		size_x_ = 0;
		size_y_ = 0;
	}

	base_entity(
		const int& value_coordinate_x,
		const int& value_coordinate_y,
		const int& value_size_x,
		const int& value_size_y
	) : 
		coordinate_x_(value_coordinate_x),
		coordinate_y_(value_coordinate_y),
		size_x_(value_size_x),
		size_y_(value_size_y)
	{

		coordinate_x_ = value_coordinate_x;
		coordinate_y_ = value_coordinate_y;
		size_x_ = value_size_x;
		size_y_ = value_size_y;

	}

	base_entity(
		const int& value_coordinate_x,
		const int& value_coordinate_y
	) :
		coordinate_x_(value_coordinate_x),
		coordinate_y_(value_coordinate_y),
		size_x_(0),
		size_y_(0)
	{
	}

protected:

	//getters
	int get_coordinate_x() const
	{
		return coordinate_x_;
	}

	int get_coordinate_y() const
	{
		return coordinate_y_;
	}

	int get_size_x() const
	{
		return size_x_;
	}

	int get_size_y() const
	{
		return size_y_;
	}

	//setters
	void set_coordinate_x(
		const int& value
	)
	{
		this->coordinate_x_ = value;
	}

	void set_coordinate_y(
		const int& value
	)
	{
		this->coordinate_y_ = value;
	}

	void set_size_x(
		const int& value
	)
	{
		this->size_x_ = value;
	}

	void set_size_y(
		const int& value
	)
	{
		this->size_y_ = value;
	}
};
