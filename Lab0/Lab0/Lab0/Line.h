#ifndef BASEENTITY
#define BASE_ENTITY
#include "base_entity.h"
class Line :
    public base_entity
{

public:

    Line() : base_entity()
    {
    }

    Line(
        HDC& hdc,
        const int& coordinate_x,
        const int& coordinate_y
    )
        :
        base_entity(
            coordinate_x,
            coordinate_y
        )
    {
	    Line::create(
            hdc,
			coordinate_x,
            coordinate_y
        );
    }

    void transform(
        HDC& hdc,
        const int& coordinate_x,
        const int& coordinate_y,
        const int& size_x,
        const int& size_y
    ) override;

    void move(
        HDC& hdc,
        const int& coordinate_x,
        const int& coordinate_y
    ) override;

    void resize(
        HDC& hdc,
        const int& size_x,
        const int& size_y
    ) override;
private:
    void create(
        HDC& hdc,
        const int& coordinate_x,
        const int& coordinate_y
    ) override;
};
#endif