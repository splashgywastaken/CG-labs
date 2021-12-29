#ifndef MODEL3D_IMPL_H
#define MODEL3D_IMPL_H
#include "affine_transform.h"
#include "Matrix.h"
#include "model3d.h"
#include "convert_fucntions.h"
#include "convert_functions_3d.h"
class model3d_impl final :
    public virtual model3d
{

	matrix<double> verticies_;
	matrix<int> edges_;
	matrix<int> faces_;

	point_double_3d rotation_;
	point_double_3d position_;
	point_double_3d previous_position_;

	matrix<int> get_edges();

public:

	model3d_impl(
		const std::vector<std::vector<double>>& verticies,
		const std::vector<std::vector<int>>& faces,
		const point_double_3d& init_position_adjustment,
		const point_double_3d& init_rotation_adjustment
	) 
	{

		verticies_ = matrix<double>(verticies);
		faces_ = matrix<int>(faces);
		edges_ = matrix<int>(get_edges());

		rotation_ = point_double_3d(0, 0, 0);

		for (size_t i = 0; i < verticies_.m(); i++)
		{
			const auto k = new double(verticies_[3][i]);

			verticies_[0][i] += init_position_adjustment.x() * *k;
			verticies_[1][i] += init_position_adjustment.y() * *k;
			verticies_[2][i] += init_position_adjustment.z() * *k;

			delete k;
		}

		const double k = 1 / verticies[0][3];
		position_ = point_double_3d(
			verticies_[0][0] * k,
			verticies_[0][1] * k,
			verticies_[0][2] * k
		);

		affine_transform::custom_rotation(
			init_rotation_adjustment,
			get_center() + position_,
			verticies_
		);

		rotation_ += init_rotation_adjustment;

		previous_position_ = point_double_3d(
			verticies_[0][0] * k,
			verticies_[0][1] * k,
			verticies_[0][2] * k
		);
	}

	~model3d_impl() override
	= default;

	model3d_impl(const model3d_impl& other) = default;

	model3d_impl& operator=(const model3d_impl& other)
	{
		if (this == &other)
			return *this;
		model3d::operator =(other);
		verticies_ = other.verticies_;
		edges_ = other.edges_;
		faces_ = other.faces_;
		rotation_ = other.rotation_;
		position_ = other.position_;
		previous_position_ = other.previous_position_;
		return *this;
	}

	model3d_impl& operator=(model3d_impl&& other) noexcept
	{
		if (this == &other)
			return *this;
		model3d::operator =(other);
		verticies_ = other.verticies_;
		edges_ = other.edges_;
		faces_ = other.faces_;
		rotation_ = other.rotation_;
		position_ = other.position_;
		previous_position_ = other.previous_position_;
		return *this;
	}
	
	void draw(
		const HDC& dc,
		matrix<double>& world_to_projection_matrix,
		point_double point_world0_in_screen,
		point_double pixel_density
	) override;

	void resize(HDC& dc, point_double_3d new_size) override;

	void move(HDC& dc, point_double_3d vector) override;

	void rotate(
		HDC& dc,
		rotation_type_3d rotation_type,
		double radian
	) override;

	void mapping(HDC& dc, mirror_type_3d mirror_type) override;

	void custom_affine(HDC& dc) override;

	point_double_3d get_center()
	{

		point_double_3d center_point{ 0.0,0.0,0.0 };
		const size_t n = verticies_.n();

		double k;
		double sum_x = 0;
		double sum_y = 0;
		double sum_z = 0;

		for(size_t i = 0; i < n; i++)
		{
			k = 1.0 / verticies_[i][3];
			sum_x += verticies_[i][0] * k;
			sum_y += verticies_[i][1] * k;
			sum_z += verticies_[i][2] * k;
		}

		k = 1.0 / n;

		center_point.set(
			sum_x * k,
			sum_y * k,
			sum_z * k
		);

		return center_point;
	}

};
#endif