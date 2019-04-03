#pragma once
#include "ColoredPoint.h"
#include "BoundingBox.h"
#include "HitInfo.h"
#include "Constants.h"
#include <algorithm>
using namespace std;
//Triangle Representation
struct Triangle {
	int idx;					//keep track of which triangle in which BB (begins at 1 for consistency)
	ColoredPoint p1, p2, p3;
	Point center;			//points that make up triangle

	BoundingBox getBBox() {
		double minx = min(p1.x, min(p2.x, p3.x));
		double miny = min(p1.y, min(p2.y, p3.y));
		double minz = min(p1.z, min(p2.z, p3.z));

		double maxx = max(p1.x, max(p2.x, p3.x));
		double maxy = max(p1.y, max(p2.y, p3.y));
		double maxz = max(p1.z, max(p2.z, p3.z));


		return BoundingBox(Point(minx, miny, minz), Point(maxx, maxy, maxz));
	}

	inline Point getPoint(int i) {
		if (i == 0) {
			return p1;
		}
		if (i == 1) {
			return p2;
		}
		else return p3;
	}

	bool hit(const Ray &r, double & min_t, HitInfo & hit_info) const {
		double a = p1.x - p2.x;
		double b = p1.x - p3.x;
		double c = r.direction.x;
		double d = p1.x - r.origin.x;
		double e = p1.y - p2.y;
		double f = p1.y - p3.y;
		double g = r.direction.y;
		double h = p1.y - r.origin.y;
		double i = p1.z - p2.z;
		double j = p1.z - p3.z;
		double k = r.direction.z;
		double l = p1.z - r.origin.z;

		double denom = 1. / (a * (f * k - g * j) + b * (g * i - e * k) + c * (e * j - f * i));

		double beta = (d * (f * k - g * j) + b * (g * l - h * k) + c * (h * j - f * l)) * denom;

		if (beta < 0.) {
			return false;
		}

		double gamma = (a * (h * k - g * l) + d * (g * i - e * k) + c * (e * l - h * i)) * denom;
		if (gamma < 0.) {
			return false;
		}

		if (gamma + beta > 1.) {
			return false;
		}

		double t = (a * (f * l - h * j) + b * (h * i - e * l) + d * (e * j - f * i)) * denom;
		if (t < TINY_DOUBLE) {
			return false;
		}

		hit_info.didHit = true;
		min_t = t;
		return true;
	}


	Triangle() {};
	Triangle(ColoredPoint p1_arg, ColoredPoint p2_arg, ColoredPoint p3_arg, int idx_arg) : p1(p1_arg), p2(p2_arg), p3(p3_arg), idx(idx_arg) {
		float xCenter = (max(p1.x, max(p2.x, p3.x)) + min(p1.x, min(p2.x, p3.x))) / 2.0f;
		float yCenter = (max(p1.y, max(p2.y, p3.y)) + min(p1.y, min(p2.y, p3.y))) / 2.0f;
		float zCenter = (max(p1.z, max(p2.z, p3.z)) + min(p1.z, min(p2.z, p3.z))) / 2.0f;
		center = Point(xCenter, yCenter, zCenter);
	}
};

