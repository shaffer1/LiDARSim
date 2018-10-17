#pragma once
#include "BoundingBox.h"
class LinearBVHNode
{
public:
	BoundingBox bbox;
	union {
		int obj_offset;
		int child_offset;
	};
	uint8_t num_objs;
	uint8_t axis;
	uint8_t pad[2];
	LinearBVHNode();
	~LinearBVHNode();
};


