#include "BVHNode.h"



BVHNode::BVHNode(BoundingBox b) : boundingBox(b), index(-1), leftIndex(-1), rightIndex(-1), triangleIndex(-1)
{
}


BVHNode::~BVHNode()
{
}
