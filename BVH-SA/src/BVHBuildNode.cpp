#include "BVHBuildNode.h"


void BVHBuildNode::init_leaf(int first, int num, const BoundingBox & b)
{
	first_obj_offset = first;
	num_objs = num;
	bounds = b;
}

void BVHBuildNode::init_interior(int axis, BVHBuildNode * c0, BVHBuildNode * c1)
{
	children[0] = c0;
	children[1] = c1;
	split_axis = axis;
	num_objs = 0;
	bounds = c0->bounds.union_boxes(c1->bounds);
}

BVHBuildNode::BVHBuildNode()
{
	children[0] = children[1] = NULL;
}


BVHBuildNode::~BVHBuildNode()
{
}

