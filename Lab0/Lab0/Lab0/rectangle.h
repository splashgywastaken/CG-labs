#ifndef RECTANGLE
#define RECTANGLE
#include "base_entity.h"
#include <vector>
class rectangle :
    public base_entity
{
public:
    static void set_offsets(int coordinates[2][4], const std::vector<POINT>& mouse_pos);

    rectangle() : base_entity()
    {

    }

    rectangle(
        const int& value_coordinate_x,
        const int& value_coordinate_y,
        const int& value_size_x,
        const int& value_size_y
    )
        : base_entity(
            value_coordinate_x,
            value_coordinate_y,
            value_size_x,
            value_size_y
        )
    {
    }

    void transform(
        HDC& hdc,
        const int& coordinate_x,
        const int& coordinate_y,
        const int& size_x,
        const int& size_y
    ) override;

    void move(
        int offsetX,
        int offsetY
    ) override;

    void resize(
        int resize_param
    ) override;

    int create(
        HDC& hdc
    );

    void create(HDC& hdc, const int& coordinate_first_x, const int& coordinate_first_y, const int& coordinate_second_x,
	    const int& coordinate_second_y) override;
    void clear(HDC dc) override;
    void create(HDC& hdc, const int& coordinate_x, const int& coordinate_y) override;
};
#endif