#ifndef BASEENTITY
#define BASEENTITY
#include "BaseEntity.h"
class Line :
    private BaseEntity
{

public:

    Line() : BaseEntity()
    {
    }

    Line(
        const double& coordinate_x,
        const double& coordinate_y,
        const int& size_x,
        const int& size_y
    )
        :
        BaseEntity(
            coordinate_x,
            coordinate_y,
            size_x,
            size_y
        )
    {
    }

    void transform(
        HDC& hdc,
        const double& coordinate_x,
        const double& coordinate_y,
        const int& size_x,
        const int& size_y
    ) override;

    void move(
        HDC& hdc,
        const double& coordinate_x,
        const double& coordinate_y
    ) override;

    void resize(
        HDC& hdc,
        const int& size_x,
        const int& size_y
    ) override;
};
#endif