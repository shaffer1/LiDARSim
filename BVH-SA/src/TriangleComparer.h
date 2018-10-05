#pragma once
#include "Triangle.h"
class TriangleComparer
{
public:
	int dimension; // X = 0, Y = 1, Z = 2
	bool operator()(Triangle * a, Triangle * b);
	TriangleComparer();
	~TriangleComparer();
};

