#pragma once
#include "BoundingBox.h"
class BVHBuildNode
{
public:
	BoundingBox bounds;
	BVHBuildNode* children[2];
	int split_axis, first_obj_offset, num_objs;

	void init_leaf(int first, int num_objs, const BoundingBox &b);
	void init_interior(int axis, BVHBuildNode* c0, BVHBuildNode* c1);

	BVHBuildNode();
	~BVHBuildNode();
};

