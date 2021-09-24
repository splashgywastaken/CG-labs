#pragma once
#include "base_entity.h"
class square :
    public base_entity
{

    square() : base_entity()
    {

    }

public:
    square(
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

    square(
        HDC& hdc,
        const int& value_coordinate_x,
        const int& value_coordinate_y
    )
        : base_entity(
            value_coordinate_x,
            value_coordinate_y
        )
    {

        this->create(
            hdc,
            value_coordinate_x,
            value_coordinate_y
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

