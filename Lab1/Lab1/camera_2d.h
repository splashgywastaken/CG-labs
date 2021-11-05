#ifndef CAMERA_2D
#define CAMERA_2D

#include "convert_fucntions.h"
#include "PointDouble.h"
#include "scene_object.h"


class camera_2d final :
    virtual public scene_object
{
    
    int W, H; //���������� ����

	double L, R, T, B; //������� ����������

    double pixel_x, pixel_y; //������ ������� �� ��������� � ������� �����������
	POINT point_world0_in_screen{}; //�������� ���������� ������� ������� ���������

    bool is_dragging_;
    POINT delta{0,0};

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
	    : W(w),
	      H(h),
		  L(l),
		  R(r),
		  T(t),
	      B(b)
    {

        is_dragging_ = false;

        //������������ ������� ������� ������� ��� �������������
		pixel_x = 
            W / (abs(r) + abs(l))
            ;
		pixel_y =
            H / (abs(t) + abs(b))
            ;

        //���������� ����� ������ ������� ��������� � �������� �����������
        point_world0_in_screen.x = static_cast<LONG>((p0.x - l) / (r - l) * W);
        point_world0_in_screen.y = static_cast<LONG>((t - p0.y) / (t - b) * H);

    }

    //�������� �������
    //������������ ��� ��������� ������� ����
    void on_window_size_change(int W, int H);

    //Window clear
    static void clear_window(HDC hdc);
    
    //���������� ������ ������� � ������ ��� 
    void draw(HDC dc, graph_2d graph) override;

    //��������� �� ������ ������� ���� ���������� ������� ������� ������ ������� ��� ��������� ��� �������� 
    int resize(double new_size, POINT screen_point) override;

    //��������� �� ������ ������� ���� ���������� ������� ������� ������ ������� ��� ��������� ��� ���������
    //����� ������������ ��� ����������� ������� ������ (���� � ������� �������)
    int move_to(POINT new_point, graph_2d graph) override;
    
    //��������������� �������
    //����� ��� ����������� �������� �� ������� ������ 
    bool is_inner_point(POINT point, graph_2d graph) override;

    void start_dragging(PointDouble drag_point)
    {

        is_dragging_ = true;
        delta.x = static_cast<LONG>(point_world0_in_screen.x - drag_point.x);
        delta.y = static_cast<LONG>(point_world0_in_screen.y - drag_point.y);

    }

    void drag(PointDouble drag_point)
    {

        point_world0_in_screen.x = static_cast<LONG>(delta.x + drag_point.x);
        point_world0_in_screen.y = static_cast<LONG>(delta.y + drag_point.y);

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
        return - static_cast<double>(point_world0_in_screen.x) / pixel_x;
    }

	double get_r() const
	{
        return (static_cast<double>(W) - static_cast<double>(point_world0_in_screen.x)) / pixel_x;
    }

	double get_t() const
	{
        return static_cast<double>(point_world0_in_screen.y) / pixel_y;
    }

	double get_b() const
	{
        return ( static_cast<double>(point_world0_in_screen.x) - static_cast<double>(H) ) / pixel_y;
    }



};
#endif