#pragma once
#include "Point.h"
#include "Ray.h"
#include <cmath>
#include <algorithm>
using namespace std;
//Axis-Aligned Bounding Box (BB)
	//faces always parallel to axes
	//region R = { (x, y, z) | |c.x-x|<=rx, |c.y-y|<=ry, |c.z-z|<=rz }
struct BoundingBox {
	Point min;
	Point max;
	bool empty;

	BoundingBox() {
		empty = true;
	}
	BoundingBox(const Point & min, const Point & max) : min(min), max(max) { empty = false; }

	BoundingBox union_boxes(const BoundingBox & b2)
	{
		Point new_min = Point::min(this->min, b2.min);
		Point new_max = Point::max(this->max, b2.max);
		return BoundingBox(new_min, new_max);
	}

	BoundingBox union_boxes(const Point & p)
	{
		if (empty) {
			empty = false;
			return BoundingBox(p, p);
		}
		Point new_min = Point::min(this->min, p);
		Point new_max = Point::max(this->max, p);
		return BoundingBox(new_min, new_max);
	}

	int getMaximumDimension() {
		double xdiameter = max.x - min.x;
		double ydiameter = max.y - min.y;
		double zdiameter = max.z - min.z;

		if (xdiameter >= ydiameter && xdiameter >= zdiameter) {
			return 0;
		}
		if (ydiameter >= xdiameter && ydiameter >= zdiameter) {
			return 1;
		}
		return 2;
	}

	//inspired from Smit's method http://www.cs.utah.edu/~awilliam/box/box.pdf
	bool hit(const Ray& r) const {
		double divx = 1. / r.direction.x;
		double xnear = (min.x - r.origin.x) * divx;
		double xfar = (max.x - r.origin.x) * divx;

		if (xfar < xnear) {
			std::swap(xfar, xnear);
		}

		double divy = 1. / r.direction.y;
		double ynear = (min.y - r.origin.y) * divy;
		double yfar = (max.y - r.origin.y) * divy;

		if (yfar < ynear) {
			std::swap(yfar, ynear);
		}

		double divz = 1. / r.direction.z;
		double znear = (min.z - r.origin.z) * divz;
		double zfar = (max.z - r.origin.z) * divz;

		if (zfar < znear) {
			std::swap(zfar, znear);
		}

		if (ynear > xfar || xnear > yfar) {
			return false;
		}

		if (ynear > xnear) {
			xnear = ynear;
		}
		if (yfar < xfar) {
			xfar = yfar;
		}

		if (xnear > zfar || znear > xfar) {
			return false;
		}

		if (znear > xnear) {
			xnear = znear;
		}
		if (zfar < xfar) {
			xfar = zfar;
		}

		return xfar > 0;
	}
};