#ifndef MODEL3D_IMPL_H
#define MODEL3D_IMPL_H
#include "affine_transform.h"
#include "Matrix.h"
#include "model3d.h"
#include "convert_functions_3d.h"
class model3d_impl final :
    public virtual model3d
{

	matrix<double> vertices_;
	matrix<int> edges_;
	matrix<int> faces_;

	point_double_3d rotation_;
	point_double_3d position_;

	matrix<int> get_edges();

public:
	model3d_impl* clone() const override
	{
		return new model3d_impl(*this);
	}

	model3d_impl(
		const std::vector<std::vector<double>>& vertices,
		const std::vector<std::vector<int>>& faces,
		const point_double_3d init_position_adjustment,
		const point_double_3d init_rotation_adjustment
	) : rotation_(init_rotation_adjustment)
	{

		vertices_ = matrix<double>(vertices);
		faces_ = matrix<int>(faces);
		edges_ = matrix<int>(get_edges());


		vertices_ = affine_transform::move(vertices_, init_position_adjustment);
	
		const double k = 1 / vertices[3][ faces_[0][0] ];
		position_ = point_double_3d(
			vertices_[0][ faces_[0][0] - static_cast<size_t>(1) ] * k,
			vertices_[1][ faces_[0][0] - static_cast<size_t>(1) ] * k,
			vertices_[2][ faces_[0][0] - static_cast<size_t>(1) ] * k
		);

		for (size_t i = 0; i < 3; i++)
		{
			//Перевод градусов в радианы
			rotation_[i] *= PI/180;
		}

		if (rotation_[0] != 0.0 || rotation_[1] != 0.0 || rotation_[2] != 0.0) {
			affine_transform::custom_rotation(
				rotation_,
				get_center(),
				vertices_
			);
		}
		
	}

	~model3d_impl() override;

	model3d_impl(const model3d_impl& other);

	model3d_impl& operator=(const model3d_impl& other)
	{
		if (this == &other)
			return *this;
		model3d::operator =(other);
		vertices_ = other.vertices_;
		edges_ = other.edges_;
		faces_ = other.faces_;
		rotation_ = other.rotation_;
		position_ = other.position_;
		return *this;
	}

	model3d_impl& operator=(model3d_impl&& other) noexcept
	{
		if (this == &other)
			return *this;
		model3d::operator =(other);
		vertices_ = other.vertices_;
		edges_ = other.edges_;
		faces_ = other.faces_;
		rotation_ = other.rotation_;
		position_ = other.position_;
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
		const size_t m = vertices_.m();

		double k;

		for(size_t i = 0; i < m; i++)
		{
			k = 1.0 / vertices_[3][i];
			center_point[0] += vertices_[0][i] * k;
			center_point[1] += vertices_[1][i] * k;
			center_point[2] += vertices_[2][i] * k;
		}

		k = 1.0 / m;

		center_point *= k;

		return center_point;
	}

};
#endif