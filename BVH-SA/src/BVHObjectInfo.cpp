#include "BVHObjectInfo.h"


BVHObjectInfo::BVHObjectInfo(int id, const BoundingBox & bbox) : id(id), bbox(bbox)
{
	center = bbox.min * .5 +  bbox.max * .5;
}

BVHObjectInfo::~BVHObjectInfo()
{
}

