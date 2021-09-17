#ifndef VIDEOENGINEOBJECT
#define VIDEOENGINEOBJECT
#include <Windows.h>
class VideoEngineObject {

public:
	virtual ~VideoEngineObject() = 0;

	virtual void transform(
		HDC& hdc,
		const double& coordinate_x,
		const double& coordinate_y,
		const int& size_x,
		const int& size_y
	) = 0;

	virtual void move(
		HDC& hdc,
		const double& coordinate_x,
		const double& coordinate_y
	) = 0;

	virtual void resize(
		HDC& hdc,
		const int& size_x,
		const int& size_y
	) = 0;
};
#endif
