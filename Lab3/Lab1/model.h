#ifndef MODEL_H
#define MODEL_H
#include "framework.h"
#include "PointDouble.h"

class model
{

public:
	virtual ~model() = default;
	virtual void draw(
		HDC& dc,
		point_double point0_in_world,
		point_double pixel_density,
		double l,
		double r
	) = 0;
	virtual void resize(HDC& dc, point_double new_size) = 0;
	virtual void move(HDC& dc, point_double vector) = 0;
	virtual void rotate(HDC& dc, POINT rotation_type) = 0;
	virtual void rotate(HDC& dc, double radian) = 0;
	virtual void rotate(HDC& dc, double cos, double sin) = 0;
	virtual void custom_affine(HDC& dc) = 0;

};
#endif
