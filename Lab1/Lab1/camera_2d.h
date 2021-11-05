#ifndef CAMERA_2D
#define CAMERA_2D

#include "convert_fucntions.h"
#include "PointDouble.h"
#include "scene_object.h"


class camera_2d final :
    virtual public scene_object
{
    
    int W, H; //Разрешение окна

	double L, R, T, B; //Мировые координаты

    double pixel_x, pixel_y; //Размер пикселя по отношению к мировым координатам
	POINT point_world0_in_screen{}; //Экранные координаты мировой системы координат

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

        //Единоразовое задание размера пикселя при инициализации
		pixel_x = 
            W / (abs(r) + abs(l))
            ;
		pixel_y =
            H / (abs(t) + abs(b))
            ;

        //Координаты точки центра мировых координат в экранных координатах
        point_world0_in_screen.x = static_cast<LONG>((p0.x - l) / (r - l) * W);
        point_world0_in_screen.y = static_cast<LONG>((t - p0.y) / (t - b) * H);

    }

    //Основные функции
    //Используется при изменении размера окна
    void on_window_size_change(int W, int H);

    //Window clear
    static void clear_window(HDC hdc);
    
    //Использует данные графика и рисует его 
    void draw(HDC dc, graph_2d graph) override;

    //Ссылается на данные графика либо использует функцию ресайза самого графика для изменения его размеров 
    int resize(double new_size, POINT screen_point) override;

    //Ссылается на данные графика либо использует функцию ресайза самого графика для изменения его положения
    //Будет использована при перемещении графика мышкой (либо с помощью стрелок)
    int move_to(POINT new_point, graph_2d graph) override;
    
    //Вспомогательные функции
    //Нужна для определения является ли текущий график 
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
    
    //Нужна для определения с каким именно объектом работаем, т.к. используется наследование
    //от абстрактного класса
    std::string to_string() override;

private:
    //Функция для задания координатных прямых
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