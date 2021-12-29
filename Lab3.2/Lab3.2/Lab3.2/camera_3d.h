#ifndef CAMERA_3D_H
#define CAMERA_3D_H
#include <utility>

#include "convert_fucntions.h"
#include "Matrix.h"
#include "model3d.h"
#include "PointDouble.h"
#include "point_double_3d.h"
#include "vector_3d.h"

class camera_3d
{
	//����� �� �������� ���������, ������������ ����������� �����������
	point_double_3d point_ov_;
	//������� ���������� ������� ������� �������� ���������,
	//����������� �� ����� Ov � �����������
	vector_3d vector_n_;
	//������� ���������� �������, ��������� ������������ ����������� ��� �����������, ������ !(T||N)
	vector_3d vector_t_;

	//������� ������ ������� ������� ��������� 
	vector_3d iv_vector_;
	vector_3d jv_vector_;
	vector_3d kv_vector_;

	double f_;

	matrix<double> world_to_projection_matrix_;

	point_double delta_{ 0,0 };
	bool is_dragging_ = false;

	//������������ ������� ������� ����� � �������� �����������
	point_double point_world0_in_screen_;

	//���������� ��� ������ � �������:
	//���������� ������
	double w_;
	double h_;
	//������ �������:
	point_double pixel_size_;

	void update();

public:
	camera_3d(
		point_double_3d point_ov, 
		vector_3d vector_n,
		vector_3d vector_t, 
		const double f,
		const point_double point_world0, 
		double const w,
		double const h,
		point_double pixel_size
	)
		: point_ov_(std::move(point_ov)),
		  vector_n_(std::move(vector_n)),
		  vector_t_(std::move(vector_t)),
		  iv_vector_(0,0,0),
		  jv_vector_(0,0,0),
		  kv_vector_(0,0,0),
		  f_(f),
		  w_(w),
		  h_(h),
		  pixel_size_(std::move(pixel_size))
	{
		point_world0_in_screen_ = {
			point_world0.x * pixel_size.x + w_ * 0.5,
			point_world0.y * pixel_size.y + h_ * 0.5
		};

		iv_vector_ = vector_3d(0, 0, 0);
		jv_vector_ = vector_3d(0, 0, 0);
		kv_vector_ = vector_3d(0, 0, 0);
		update();

	}

	~camera_3d()
	{
		world_to_projection_matrix_.clear();
	}

	camera_3d(const camera_3d& other) = default;

	camera_3d(camera_3d&& other) noexcept
		: point_ov_(other.point_ov_),
		  vector_n_(other.vector_n_),
		  vector_t_(other.vector_t_),
		  iv_vector_(other.iv_vector_),
		  jv_vector_(other.jv_vector_),
		  kv_vector_(other.kv_vector_),
		  f_(other.f_),
		  point_world0_in_screen_(other.point_world0_in_screen_),
		  w_(other.w_),
		  h_(other.h_),
		  pixel_size_(other.pixel_size_)
	{
	}

	camera_3d& operator=(camera_3d&& other) noexcept
	{
		if (this == &other)
			return *this;
		point_ov_ = other.point_ov_;
		vector_n_ = other.vector_n_;
		vector_t_ = other.vector_t_;
		iv_vector_ = other.iv_vector_;
		jv_vector_ = other.jv_vector_;
		kv_vector_ = other.kv_vector_;
		f_ = other.f_;
		point_world0_in_screen_ = other.point_world0_in_screen_;
		w_ = other.w_;
		h_ = other.h_;
		pixel_size_ = other.pixel_size_;
		return *this;
	}

	//��������� �� ������ ������� ���� ���������� ������� ������� ������ ������� ��� ��������� ��� �������� 
	int resize(double new_size, POINT screen_point);

	void start_dragging(const point_double drag_point)
	{

		is_dragging_ = true;
		delta_.x = point_world0_in_screen_.x - drag_point.x;
		delta_.y = point_world0_in_screen_.y - drag_point.y;

	}

	void drag(const point_double drag_point)
	{
		point_world0_in_screen_.x = delta_.x + drag_point.x;
		point_world0_in_screen_.y = delta_.y + drag_point.y;
	}

	void stop_dragging()
	{

		is_dragging_ = false;

	}

	bool is_dragging() const
	{
		return is_dragging_;
	}

	//������ ������ ��� ������ � ����� ������
	void on_window_size_change(int w, int h);
	//Window clear
	static void clear_window(const HDC& hdc);
	//����� ��������� ��������:
	void draw(const HDC& hdc, model3d* model);

	//������� � �������:

	//Ov
	point_double_3d ov() const
	{
		return point_ov_;
	}
	void set_ov(const point_double_3d& ov)
	{
		point_ov_ = ov;
		update();
	}
	//T
	vector_3d get_vector_t() const
	{
		return vector_t_;
	}
	void set_vector_t(const vector_3d& t)
	{
		vector_t_ = t;
		update();
	}
	//N
	vector_3d n() const
	{
		return vector_n_;
	}
	void set_n(const vector_3d& n)
	{
		vector_n_ = n;
		update();
	}
};
#endif