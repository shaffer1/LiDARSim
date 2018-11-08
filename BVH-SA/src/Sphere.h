#pragma once
#include "Point.h"
#include "Plane.h"
class Sphere
{
public:
	Point center;
	float radius;
	Sphere();
	~Sphere();

	bool insidePlane(const Plane & p) const {
		return -p.planeDistance(center) >= radius;
	}

	bool outsidePlane(const Plane & p) const {
		return p.planeDistance(center) > radius;
	}

	bool intersectsPlane(const Plane & p) const {
		return p.planeDistance(center) <= radius;
	}

};

