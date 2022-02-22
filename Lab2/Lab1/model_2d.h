#ifndef MODEL_2D_H
#define MODEL_2D_H

#include "Matrix.h" 
#include "model.h"
#include "PointDouble.h"

class model_2d final : public model 
{
	//¬ершины модели хран€тс€ в матрице размера 3 на M (где M Ц число вершин модели).
	//¬ каждом столбце хран€тс€ однородные координаты соответствующей вершины.
	matrix<double> verticies_;
	matrix<double> verticies_buff_;
	//матрица смежности размера M на M, состо€щей из нулей и единиц,
	//при этом на пересечении i-ой строки и j-ого столбца стоит единица,
	//если i-а€ и j-а€ вершины соединены ребром
	matrix<int> edges_;

	point_double centre_point_{ 0,0 };
	matrix<double> previous_point_{{0,0,1}};
	double rotation_radian_ = 0;

public:
	model_2d(
		const matrix<double>& verticies,
		const matrix<int>& edges
	) :
		verticies_(verticies),
		verticies_buff_(verticies),
		edges_(edges)
	{
		centre_point_ = get_centre();
	}

	model_2d(
		const std::vector<std::vector<double>>& verticies,
		const std::vector<std::vector<int>>& edges
	)
		:
		verticies_(verticies),
		verticies_buff_(verticies),
		edges_(edges)
	{
		centre_point_ = get_centre();
	}

	void draw(
		HDC dc,
		POINT point0_in_world,
		point_double pixel,
		double l,
		double r
	) override;
	void resize(HDC dc, point_double new_size) override;
	void custom_resize(HDC dc, point_double new_size);
	void move(HDC dc, point_double vector) override;
	void rotate(HDC dc, POINT rotation_type) override;
	void rotate(HDC dc, double radian) override;
	void custom_rotate(HDC dc, double radian);
	void rotate(HDC dc, double cos, double sin) override;
	void custom_affine(HDC dc) override;
private:
	point_double get_centre();
	double get_avg_x();
	double get_avg_y();
	double get_fat_right();
	double get_far_left();
	double get_far_top();
	double get_far_bottom();
};
#endif