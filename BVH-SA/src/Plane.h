#pragma once
#include "Point.h"
class Plane
{
public:
	Point point;
	Point direction;
	Plane();
	~Plane();

	float planeDistance(const Point& p) const {
		return direction.dot(p - point);
	}
};

