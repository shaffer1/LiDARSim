#pragma once
#include "BoundingBox.h"
class BVHNode
{
public:
	// TODO: Compress this down a bit. Some redundant data isnt needed
	BoundingBox boundingBox;
	bool isLeaf;
	size_t numTriangles;
	bool taken = false;
	size_t index;
	size_t leftIndex;
	size_t rightIndex;
	size_t triangleIndex;

	BVHNode() : index(-1), leftIndex(-1), rightIndex(-1), triangleIndex(-1) {};
	BVHNode(BoundingBox b);
	~BVHNode();
};

