#ifndef	MODEL3D_H
#define MODEL3D_H
#include "Camera2D.h"
#include "mirror_type_3d.h"
#include "PointDouble.h"
#include "point_double_3d.h"
#include "rotation_type_3d.h"

class model3d
{
public:
	virtual ~model3d() = default;

	virtual void draw(
		const HDC& dc,
		matrix<double>& world_to_projection_matrix,
		point_double point_world0_in_screen,
		point_double pixel_density
	) = 0;
	virtual void resize(
		HDC& dc,
		point_double_3d new_size
	) = 0;

	virtual void move(
		HDC& dc, 
		point_double_3d vector
	) = 0;

	virtual void rotate(
		HDC& dc, 
		rotation_type_3d rotation_type,
		double radian
	) = 0;
	virtual void mapping(
		HDC& dc,
		mirror_type_3d mirror_type
	) = 0;

	virtual void custom_affine(HDC& dc) = 0;

};
#endif