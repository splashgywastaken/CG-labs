#ifndef CAMERA_2D
#define CAMERA_2D

#include "convert_fucntions.h"
#include "model.h"
#include "PointDouble.h"
#include "scene_object.h"


class camera_2d final :
    virtual public scene_object
{
	int w_;
	int h_;

	double l_;
    double r_;

	double pixel_x_;
	double pixel_y_;
	point_double point_world0_in_screen_{}; //�������� ���������� ������� ������� ���������

    bool is_dragging_;
    POINT delta_{0,0};

public:
    camera_2d(
        const int w,
        const int h,
        const double l,
        const double r,
        const double t,
        const double b,
        const POINT p0
    )
	    : w_(w),
	      h_(h),
		  l_(l),
		  r_(r),
		pixel_x_(w_ / abs(r - l)),
		pixel_y_(h_ / abs(t - b)),
		is_dragging_(false)
    {
	    //������������ ������� ������� ������� ��� �������������


        //���������� ����� ������ ������� ��������� � �������� �����������
        point_world0_in_screen_.x = static_cast<LONG>((p0.x - l) / (r - l) * w_);
        point_world0_in_screen_.y = static_cast<LONG>((t - p0.y) / (t - b) * h_);
    }

    //�������� �������
    //������������ ��� ��������� ������� ����
    void on_window_size_change(int W, int H);

    //Window clear
    static void clear_window(HDC hdc);
    
    //���������� ������ ������� � ������ ��� 
    void draw(HDC dc, graph_2d graph) override;

    //���������� ������ ������ � ������������ �
    void draw(HDC dc, model* object) const;

    //��������� �� ������ ������� ���� ���������� ������� ������� ������ ������� ��� ��������� ��� �������� 
    int resize(double new_size, POINT screen_point) override;

    //��������� �� ������ ������� ���� ���������� ������� ������� ������ ������� ��� ��������� ��� ���������
    //����� ������������ ��� ����������� ������� ������ (���� � ������� �������)
    int move_to(POINT new_point, graph_2d graph) override;
    
    //��������������� �������
    //����� ��� ����������� �������� �� ������� ������ 
    bool is_inner_point(POINT point, graph_2d graph) override;

    void start_dragging(point_double drag_point)
    {

        is_dragging_ = true;
        delta_.x = static_cast<LONG>(point_world0_in_screen_.x - drag_point.x);
        delta_.y = static_cast<LONG>(point_world0_in_screen_.y - drag_point.y);

    }

    void drag(point_double drag_point)
    {

        point_world0_in_screen_.x = static_cast<LONG>(delta_.x + drag_point.x);
        point_world0_in_screen_.y = static_cast<LONG>(delta_.y + drag_point.y);

    }

    void stop_dragging()
    {

    	is_dragging_ = false;

    }

    bool is_dragging() const
    {
	    return is_dragging_;
    }
    
    //����� ��� ����������� � ����� ������ �������� ��������, �.�. ������������ ������������
    //�� ������������ ������
    std::string to_string() override;

private:
    //������� ��� ������� ������������ ������
    void axis(HDC dc) const;

    double get_l() const
    {
        return - static_cast<double>(point_world0_in_screen_.x) / pixel_x_;
    }

	double get_r() const
	{
        return (static_cast<double>(w_) - static_cast<double>(point_world0_in_screen_.x)) / pixel_x_;
    }

	double get_t() const
	{
        return static_cast<double>(point_world0_in_screen_.y) / pixel_y_;
    }

	double get_b() const
	{
        return ( static_cast<double>(point_world0_in_screen_.y) - static_cast<double>(h_) ) / pixel_y_;
    }



};
#endif