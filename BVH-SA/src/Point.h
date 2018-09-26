#pragma once
//3D Point Representation
struct Point {
	int idx;					//idx of point in given obj file (index begins at 1)
	float x, y, z;

	Point() {};
	Point(float x_arg, float y_arg, float z_arg, int idx_arg) : x(x_arg), y(y_arg), z(z_arg), idx(idx_arg) {}
	Point(float x_arg, float y_arg, float z_arg) : x(x_arg), y(y_arg), z(z_arg) {}
};

