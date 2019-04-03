#pragma once
#include "Point.h"
class ColoredPoint :
	public Point
{
public:
	float r, g, b;

	ColoredPoint() {};
	ColoredPoint(float x_arg, float y_arg, float z_arg, float r_arg, float g_arg, float b_arg) {
		x = x_arg;
		y = y_arg;
		z = z_arg;
		r = r_arg;
		g = g_arg;
		b = b_arg;
	};
	~ColoredPoint() {};
};

