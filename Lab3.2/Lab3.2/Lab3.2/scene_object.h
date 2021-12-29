#ifndef SCENE_OBJECT
#define SCENE_OBJECT

#include <string>
#include "framework.h"

class graph_2d;

class scene_object
{
public:

	virtual void draw(HDC dc, graph_2d graph) = 0;
	virtual bool is_inner_point(POINT point, graph_2d graph) = 0;

	virtual int resize(double new_size, POINT screen_point) = 0;
	virtual int move_to(POINT new_point, graph_2d graph) = 0;

	virtual std::string to_string() = 0;

	virtual  ~scene_object() = default;
};
#endif