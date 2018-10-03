#pragma once
#include "Point.h"
#include <algorithm>
using namespace std;
//Triangle Representation
struct Triangle {
	int idx;					//keep track of which triangle in which BB (begins at 1 for consistency)
	Point p1, p2, p3, center;			//points that make up triangle


	Triangle() {};
	Triangle(Point p1_arg, Point p2_arg, Point p3_arg, int idx_arg) : p1(p1_arg), p2(p2_arg), p3(p3_arg), idx(idx_arg) {
		float xCenter = (max(p1.x, max(p2.x, p3.x)) + min(p1.x, min(p2.x, p3.x))) / 2.0f;
		float yCenter = (max(p1.y, max(p2.y, p3.y)) + min(p1.y, min(p2.y, p3.y))) / 2.0f;
		float zCenter = (max(p1.z, max(p2.z, p3.z)) + min(p1.z, min(p2.z, p3.z))) / 2.0f;
		center = Point(xCenter, yCenter, zCenter);
	}
};

