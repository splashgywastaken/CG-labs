#ifndef CAMERA_2D
#define CAMERA_2D

#include "scene_object.h"

class camera_2d :
    virtual public scene_object
{
    
    int W, H; //���������� ����

	double L, R, T, B; //������� ����������
    double px_x, px_y; //������ ������� �� ��������� � ������� �����������
	POINT p_w0; //��������� ������ ������� ��������� � ����

public:
    camera_2d(int w, int h, double l, double r, double t, double b, int p0_x, int p0_y)
	    : W(w),
	      H(h),
	      L(l),
	      R(r),
	      T(t),
	      B(b)
    {

        POINT p;
        p.x = p0_x;
        p.y = p0_y;
        p_w0 = p;

        //������������ ������� ������� ������� ��� �������������
		px_x = abs (static_cast<int>((R - L) / W));
		px_y = abs (static_cast<int>((T - B) / H));

    }

    //�������� �������
    //������������ ��� ��������� ������� ����
    void on_window_size_change(int W, int H);
    
    //���������� ������ ������� � ������ ��� 
    void draw(HDC dc, graph_2d graph) override;

    //��������� �� ������ ������� ���� ���������� ������� ������� ������ ������� ��� ��������� ��� �������� 
    int resize(SIZE new_size, graph_2d graph) override;

    //��������� �� ������ ������� ���� ���������� ������� ������� ������ ������� ��� ��������� ��� ���������
    //����� ������������ ��� ����������� ������� ������ (���� � ������� �������)
    int move_to(POINT new_point, graph_2d graph) override;
    
    //��������������� �������
    //����� ��� ����������� �������� �� ������� ������ 
    bool is_inner_point(POINT point, graph_2d graph) override;

    //����� ��� ����������� � ����� ������ �������� ��������, �.�. ������������ ������������
    //�� ������������ ������
    std::string to_string() override;
};
#endif