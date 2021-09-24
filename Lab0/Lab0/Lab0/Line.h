#ifndef LINE
#define LINE
#include "base_entity.h"
class line final :
    public base_entity
{

public:

    line() : base_entity()
    {
    }

    line(
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
	    this->create(
            hdc,
			coordinate_x,
            coordinate_y
        );
    }

	line(
        HDC& hdc,
        const int& coordinate_first_x,
        const int& coordinate_first_y,
        const int& coordinate_second_x,
        const int& coordinate_second_y
    )
        :
        base_entity(
            coordinate_first_x,
            coordinate_first_y
        )
    {
	    this->create(
            hdc,
			coordinate_first_x,
            coordinate_first_y,
            coordinate_second_x,
            coordinate_second_y
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
        int coordinate_x,
        int coordinate_y
    ) override;

    void resize(
        int resize_param
    ) override;

    void clear(HDC dc) override;
private:
    void create(
        HDC& hdc,
        const int& coordinate_x,
        const int& coordinate_y
    ) override;

    void create(
        HDC& hdc,
        const int& coordinate_first_x,
        const int& coordinate_first_y,
        const int& coordinate_second_x,
	    const int& coordinate_second_y
    ) override;
};

#endif