#ifndef CONVERT_FUNCTIONS
#define CONVERT_FUNCTIONS

#include "PointDouble.h"
#include "framework.h"

static class convert_functions
{

public:

	static PointDouble world_to_screen(PointDouble pointw, int L, int R, int T, int B, int W, int H)
	{

		POINT result;

		result.x = static_cast<int>((pointw.x - L) / (R - L) * W);
		result.y = static_cast<int>((T - pointw.y) / (T - B) * H);

		return result;

	}

	static PointDouble screen_to_world(POINT points, int L, int R, int T, int B, int W, int H)
	{

		PointDouble result;

		result.x = L + (R - L) * points.x / W;
		result.y = T - (T - B) * points.y / H;

		return result;

	}
	
};

#endif
