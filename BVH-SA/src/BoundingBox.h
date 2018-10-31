#pragma once
#include "Point.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include <cmath>
#include <algorithm>
using namespace std;
//Axis-Aligned Bounding Box (BB)
	//faces always parallel to axes
	//region R = { (x, y, z) | |c.x-x|<=rx, |c.y-y|<=ry, |c.z-z|<=rz }
struct BoundingBox {
	Point min;
	Point max;

	Plane up;
	Plane down;
	Plane left;
	Plane right;
	Plane back;
	Plane front;

	bool empty;

	BoundingBox() {
		up.point = max;
		right.point = max;
		front.point = max;

		down.point = min;
		left.point = min;
		back.point = min;

		up.direction = Point(0., 1., 0.);
		right.direction = Point(1., 0., 0.);
		front.point = Point(0., 0., 1.);

		down.direction = Point(0., -1., 0.);
		left.direction = Point(-1., 0., 0.);
		back.point = Point(0., 0., -1.);

		empty = true;
	}
	BoundingBox(const Point & min, const Point & max) : min(min), max(max) { 
		up.point = max;
		right.point = max;
		front.point = max;

		down.point = min;
		left.point = min;
		back.point = min;

		up.direction = Point(0., 1., 0.);
		right.direction = Point(1., 0., 0.);
		front.point = Point(0., 0., 1.);

		down.direction = Point(0., -1., 0.);
		left.direction = Point(-1., 0., 0.);
		back.point = Point(0., 0., -1.);

		empty = false; 
	}

	bool containsPoint(const Point & p) const {
		return p >= min && p <= max;
	}
 
	bool intersect(const Point & p) const {
		return min.x <= p.x && min.y <= p.y && min.x <= p.z && max.x >= p.x && max.y >= p.y && max.z >= p.z;
	}


	bool sphereInsideBox(const Sphere & s) const {
		if (!s.insidePlane(front)) { return false; }
		if (!s.insidePlane (back)) { return false; }
		if (!s.insidePlane(up)) { return false; }
		if (!s.insidePlane(down)) { return false; }
		if (!s.insidePlane(left)) { return false; }
		if (!s.insidePlane(right)) { return false; }

		return true;
	}

	bool sphereIntersectsBox(const Sphere & s) {
		bool in_left = s.insidePlane(left);
		bool in_right = s.insidePlane(right);
		bool in_front = s.insidePlane(front);
		bool in_back = s.insidePlane(back);
		bool in_top = s.insidePlane(up);
		bool in_bottom = s.insidePlane(down);

		if (s.intersectsPlane(up) &&
			in_left && in_right && in_front && in_back) {
			return true;
		}

		if (s.intersectsPlane(down) &&
			in_left && in_right && in_front && in_back) {
			return true;
		}

		if (s.intersectsPlane(left) &&
			in_top && in_bottom && in_front && in_back) {
			return true;
		}

		if (s.intersectsPlane(right) &&
			in_top && in_bottom && in_front && in_back) {
			return true;
		}

		if (s.intersectsPlane(front) &&
			in_top && in_bottom && in_left && in_right) {
			return true;
		}

		if (s.intersectsPlane(back) &&
			in_top && in_bottom && in_left && in_right) {
			return true;
		}

		return false;
	}

	//TODO: Add intersect, not inside too
	bool intersect(const Sphere & s) const {
		return sphereInsideBox(s);
	}

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