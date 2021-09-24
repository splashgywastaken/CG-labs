#ifndef VIDEO_ENGINE_OBJECT
#define VIDEO_ENGINE_OBJECT
#include <Windows.h>
class video_engine_object {

public:

	virtual void transform(
		HDC& hdc,
		const int& coordinate_x,
		const int& coordinate_y,
		const int& size_x,
		const int& size_y
	) = 0;

	virtual void move(
		HDC& hdc,
		const int& coordinate_x,
		const int& coordinate_y
	) = 0;

	virtual void resize(
		HDC& hdc,
		const int& size_x,
		const int& size_y
	) = 0;

	virtual void clear(HDC dc) = 0;

	virtual void create(
		HDC& hdc,
		const int& coordinate_x,
		const int& coordinate_y
	) = 0;

	virtual void create(
		HDC& hdc,
		const int& coordinate_first_x,
		const int& coordinate_first_y,
		const int& coordinate_second_x,
		const int& coordinate_second_y
	) = 0;
};
#endif
