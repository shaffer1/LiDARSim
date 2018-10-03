#pragma once
#include "Point.h"
#include "Triangle.h"
#include <cmath>
#include <algorithm>
using namespace std;
//Axis-Aligned Bounding Box (BB)
	//faces always parallel to axes
	//region R = { (x, y, z) | |c.x-x|<=rx, |c.y-y|<=ry, |c.z-z|<=rz }
struct BoundingBox {
	Point center;				//center point of BB
	unsigned int mortonCode;	//morton code of center point
	float rx, ry, rz;			//radius or halfwidth extents
	int triangleIdx;			//idx of triangle BB contains
	bool isLeaf;				//true if BB is leaf in BVH...false if internal node
	BoundingBox() {};
	BoundingBox(Point center_arg, float rx_arg, float ry_arg, float rz_arg, int triangleIdx_arg, bool isLeaf_arg) : center(center_arg), rx(rx_arg), ry(ry_arg), rz(rz_arg), triangleIdx(triangleIdx_arg), isLeaf(isLeaf_arg) {}
	BoundingBox(Triangle triangle) {
		//Find center
		float xCenter = (max(triangle.p1.x, max(triangle.p2.x, triangle.p3.x)) + min(triangle.p1.x, min(triangle.p2.x, triangle.p3.x))) / 2;
		float yCenter = (max(triangle.p1.y, max(triangle.p2.y, triangle.p3.y)) + min(triangle.p1.y, min(triangle.p2.y, triangle.p3.y))) / 2;
		float zCenter = (max(triangle.p1.z, max(triangle.p2.z, triangle.p3.z)) + min(triangle.p1.z, min(triangle.p2.z, triangle.p3.z))) / 2;

		//Find radius
		rx = max((abs(xCenter - triangle.p1.x)), (max(abs(xCenter - triangle.p2.x), abs(xCenter - triangle.p3.x))));
		ry = max((abs(yCenter - triangle.p1.y)), (max(abs(yCenter - triangle.p2.y), abs(yCenter - triangle.p3.y))));
		rz = max((abs(zCenter - triangle.p1.z)), (max(abs(zCenter - triangle.p2.z), abs(zCenter - triangle.p3.z))));

		center = Point(xCenter, yCenter, zCenter);
	}
	int findLargestDimension() {
		if (rx > ry && rx > rz) {
			return 0;
		}
		else if (ry > rx && ry > rz) {
			return 1;
		}
		return 2;
	}
};


