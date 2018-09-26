#pragma once
#include "Point.h"
//Triangle Representation
struct Triangle {
	int idx;					//keep track of which triangle in which BB (begins at 1 for consistency)
	Point p1, p2, p3;			//points that make up triangle

	Triangle() {};
	Triangle(Point p1_arg, Point p2_arg, Point p3_arg, int idx_arg) : p1(p1_arg), p2(p2_arg), p3(p3_arg), idx(idx_arg) {}
};

