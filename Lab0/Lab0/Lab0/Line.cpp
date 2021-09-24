#include "line.h"

void line::transform(
    HDC& hdc,
    const int& coordinate_x,
    const int& coordinate_y,
    const int& size_x,
    const int& size_y
)
{
}

void line::move(
    HDC& hdc,
    const int& coordinate_x,
    const int& coordinate_y
)
{
    MoveToEx(hdc, coordinate_x, coordinate_y, nullptr);
}

void line::resize(
    HDC& hdc,
    const int& size_x, 
    const int& size_y)
{
}

void line::clear(HDC dc)
{
    RECT r;
    GetClientRect(WindowFromDC(dc), &r);
    Rectangle(dc, -1, -1, r.right, r.bottom);
}

void line::create(
    HDC& hdc,
    const int& coordinate_x,
    const int& coordinate_y)
{
    LineTo(hdc, coordinate_x, coordinate_y);
}

void line::create(
    HDC& hdc,
    const int& coordinate_first_x,
    const int& coordinate_first_y,
	const int& coordinate_second_x,
    const int& coordinate_second_y
)
{

    MoveToEx(
        hdc,
        coordinate_first_x,
        coordinate_first_y,
        nullptr
    );
    LineTo(
        hdc,
        coordinate_second_x,
        coordinate_second_y
    );

}
