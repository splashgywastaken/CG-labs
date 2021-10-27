#ifndef CAMERA_2D
#define CAMERA_2D

#include "scene_object.h"

class camera_2d :
    virtual public scene_object
{
    
    int W, H; //Разрешение окна

	double L, R, T, B; //Мировые координаты
    double px_x, px_y; //Размер пикселя по отношению к мировым координатам
	POINT p_w0; //Положение центра мировых координат в окне

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

        //Единоразовое задание размера пикселя при инициализации
		px_x = abs (static_cast<int>((R - L) / W));
		px_y = abs (static_cast<int>((T - B) / H));

    }

    //Основные функции
    //Используется при изменении размера окна
    void on_window_size_change(int W, int H);
    
    //Использует данные графика и рисует его 
    void draw(HDC dc, graph_2d graph) override;

    //Ссылается на данные графика либо использует функцию ресайза самого графика для изменения его размеров 
    int resize(SIZE new_size, graph_2d graph) override;

    //Ссылается на данные графика либо использует функцию ресайза самого графика для изменения его положения
    //Будет использована при перемещении графика мышкой (либо с помощью стрелок)
    int move_to(POINT new_point, graph_2d graph) override;
    
    //Вспомогательные функции
    //Нужна для определения является ли текущий график 
    bool is_inner_point(POINT point, graph_2d graph) override;

    //Нужна для определения с каким именно объектом работаем, т.к. используется наследование
    //от абстрактного класса
    std::string to_string() override;
};
#endif