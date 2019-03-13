#pragma once
#include <cmath>
//3D Point Representation
struct Point {				//idx of point in given obj file (index begins at 1)
	float x, y, z;

	Point() {};
	Point(float x_arg, float y_arg, float z_arg, int idx_arg) : x(x_arg), y(y_arg), z(z_arg) {}
	Point(float x_arg, float y_arg, float z_arg) : x(x_arg), y(y_arg), z(z_arg) {}
	inline float getDim(int dim) const {
		if (dim == 0) {
			return x;
		}
		else if (dim == 1) {
			return y;
		}
		return z;
	}

	inline float dot(const Point& b) const{
		float newx = x * b.x;
		float newy = y * b.y;
		float newz = z * b.z;
		return newx + newy + newz;
	}

	inline float dist2(const Point& p) const {
		float xdiff = x - p.x;
		float ydiff = y - p.y;
		float zdiff = z - p.z;

		return xdiff * xdiff + ydiff * ydiff + zdiff * zdiff;
	}

	inline Point operator-(float f) const {
		float newx = x - f;
		float newy = y - f;
		float newz = z - f;
		return Point(newx, newy, newz);
	}

	inline Point operator/(float f) const {
		return Point(x / f, y / f, z / f);
	}

	inline Point operator+(float f) const {
		float newx = x + f;
		float newy = y + f;
		float newz = z + f;
		return Point(newx, newy, newz);
	}

	inline bool operator>=(const Point & b) const {
		return x >= b.x && y >= b.y && z >= b.z;
	}

	inline bool operator<=(const Point & b) const {
		return x <= b.x && y <= b.y && z <= b.z;
	}

	inline Point operator+(const Point & b) const {
		float newx = x + b.x;
		float newy = y + b.y;
		float newz = z + b.z;
		return Point(newx, newy, newz);
	}

	inline Point operator-(const Point & b) const {
		float newx = x - b.x;
		float newy = y - b.y;
		float newz = z - b.z;
		return Point(newx, newy, newz);
	}

	inline Point operator*(float f) const {
		float newx = f * x;
		float newy = f * y;
		float newz = f * z;

		return Point(newx, newy, newz);
	}

	inline Point operator*(double f) const {
		double newx = f * x;
		double newy = f * y;
		double newz = f * z;

		return Point(newx, newy, newz);
	}

	inline Point inverse() const {
		float newx = 1 / x;
		float newy = 1 / y;
		float newz = 1 / z;

		return Point(newx, newy, newz);
	}

	inline Point normalize() const {
		float len = sqrtf(x * x + y * y + z * z);
		return Point(x / len, y / len, z / len);
	}

	inline static Point min(const Point & a, const Point & b) {
		float newx = a.x < b.x ? a.x : b.x;
		float newy = a.y < b.y ? a.y : b.y;
		float newz = a.z < b.z ? a.z : b.z;

		return Point(newx, newy, newz);
	}

	inline static Point max(const Point & a, const Point & b) {
		float newx = a.x > b.x ? a.x : b.x;
		float newy = a.y > b.y ? a.y : b.y;
		float newz = a.z > b.z ? a.z : b.z;

		return Point(newx, newy, newz);
	}

};

