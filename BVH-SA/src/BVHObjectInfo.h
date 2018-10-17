#pragma once
#include "BoundingBox.h"
class BVHObjectInfo
{
public:
	int id;
	Point center;
	BoundingBox bbox;

	BVHObjectInfo(int id, const BoundingBox & bbox);
	~BVHObjectInfo();
};

