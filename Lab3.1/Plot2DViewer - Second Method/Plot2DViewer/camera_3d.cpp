#include "camera_3d.h"

#include <fstream>

void camera_3d::update()
{

	kv_vector_ = vector_n_ * (1 / vector_n_.norm());
	iv_vector_ = vector_t_.vector_product(vector_n_) * (1 / vector_t_.vector_product(vector_n_).norm());
	jv_vector_ = kv_vector_.vector_product(iv_vector_);

	const auto world_to_view = new matrix<double>(std::vector<std::vector<double>>{
		{iv_vector_.x(), iv_vector_.y(), iv_vector_.z(), -point_ov_.scalar_product(iv_vector_)},
		{ jv_vector_.x(), jv_vector_.y(), jv_vector_.z(), -point_ov_.scalar_product(jv_vector_) },
		{ kv_vector_.x(), kv_vector_.y(), kv_vector_.z(), -point_ov_.scalar_product(kv_vector_) },
		{ 0, 0, 0, 1 }
	});

	matrix<double>* view_to_projection;
	if (true)
	{
		view_to_projection = new matrix<double>(std::vector<std::vector<double>>{
			{1, 0, 0, 0},
			{ 0, 1, 0, 0 },
			{ 0, 0, 0, 1 }
		});
	}
	else
	{
		view_to_projection = new matrix<double>(std::vector<std::vector<double>>{
			{1, 0, 0, 0},
			{ 0, 1, 0, 0 },
			{ 0, 0, -1 / f_, 1 }
		});
	}

	world_to_projection_matrix_ = *view_to_projection * *world_to_view;

}

camera_3d::~camera_3d(){
	world_to_projection_matrix_.clear();
}

camera_3d::camera_3d(const camera_3d& other) :
	vector_n_(other.vector_n_),
	vector_t_(other.vector_t_),
	iv_vector_(other.iv_vector_),
	jv_vector_(other.jv_vector_),
	kv_vector_(other.kv_vector_),
	f_(other.f_),
	w_(other.w_),
	h_(other.h_)
{
}

int camera_3d::resize(const double new_size, const POINT screen_point)
{

	point_world0_in_screen_.x = screen_point.x - new_size * (screen_point.x - point_world0_in_screen_.x);
	point_world0_in_screen_.y = screen_point.y - new_size * (screen_point.y - point_world0_in_screen_.y);

	pixel_size_.x = new_size * pixel_size_.x;
	pixel_size_.y = new_size * pixel_size_.y;

	return 1;

}

void camera_3d::on_window_size_change(const int w, const int h) 
{
	if (!(w_ > 0 && h_ > 0))
	{
		w_ = 1.0 + w;
		h_ = 1.0 + h;
	}

	const double w0 = w_;
	const double h0 = h_;
	w_ = 1.0 + w;
	h_ = 1.0 + h;

	const double kw = w_ / w0;
	const double kh = h_ / h0;
	const double kpx = pixel_size_.x / pixel_size_.y;

	point_world0_in_screen_.x = kw * point_world0_in_screen_.x;
	point_world0_in_screen_.x = kw * kpx * point_world0_in_screen_.y + h_ / 2 * (kh - kw * kpx);
	pixel_size_.x = kw * pixel_size_.x;
	pixel_size_.y = pixel_size_.x;
}

void camera_3d::clear_window(const HDC& hdc)
{

	RECT r;
	GetClientRect(WindowFromDC(hdc), &r);
	Rectangle(hdc, -1, -1, r.right, r.bottom);

}

void camera_3d::draw(const HDC& hdc, model3d* model)
{
	update();
	model->draw(
		hdc,
		world_to_projection_matrix_,
		point_world0_in_screen_,
		pixel_size_
	);

}
