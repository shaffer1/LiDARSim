#pragma once
#include "Point.h"

class Ray
{
public:
	Point origin;
	Point direction;

	Ray(const Point & o, const Point & d) : origin(o), direction(d) {};
	Ray() {};
	~Ray() {};
};

