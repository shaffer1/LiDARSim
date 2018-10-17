#pragma once
//3D Point Representation
struct Point {
	int idx;					//idx of point in given obj file (index begins at 1)
	float x, y, z;

	Point() {};
	Point(float x_arg, float y_arg, float z_arg, int idx_arg) : x(x_arg), y(y_arg), z(z_arg), idx(idx_arg) {}
	Point(float x_arg, float y_arg, float z_arg) : x(x_arg), y(y_arg), z(z_arg) {}
	float getDim(int dim) const {
		if (dim == 0) {
			return x;
		}
		else if (dim == 1) {
			return y;
		}
		return z;
	}

	Point operator+(const Point & b) const {
		float newx = x + b.x;
		float newy = y + b.y;
		float newz = z + b.z;
		return Point(newx, newy, newz);
	}

	Point operator*(float f) const {
		float newx = f * x;
		float newy = f * y;
		float newz = f * z;

		return Point(newx, newy, newz);
	}

	static Point min(const Point & a, const Point & b) {
		float newx = a.x < b.x ? a.x : b.x;
		float newy = a.y < b.y ? a.y : b.y;
		float newz = a.z < b.z ? a.z : b.z;

		return Point(newx, newy, newz);
	}

	static Point max(const Point & a, const Point & b) {
		float newx = a.x > b.x ? a.x : b.x;
		float newy = a.y > b.y ? a.y : b.y;
		float newz = a.z > b.z ? a.z : b.z;

		return Point(newx, newy, newz);
	}

};

