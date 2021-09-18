#include "Line.h"

void Line::transform(
    HDC& hdc,
    const int& coordinate_x,
    const int& coordinate_y,
    const int& size_x,
    const int& size_y
)
{
}

void Line::move(
    HDC& hdc,
    const int& coordinate_x,
    const int& coordinate_y
)
{
    MoveToEx(hdc, coordinate_x, coordinate_y, nullptr);
}

void Line::resize(
    HDC& hdc,
    const int& size_x, 
    const int& size_y)
{
}

void Line::create(
    HDC& hdc,
    const int& coordinate_x,
    const int& coordinate_y)
{
    LineTo(hdc, coordinate_x, coordinate_y);
}
