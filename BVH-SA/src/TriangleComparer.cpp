#include "TriangleComparer.h"
#include "BoundingBox.h"


bool TriangleComparer::operator()(Triangle & a, Triangle & b)
{
	BoundingBox aBBox = BoundingBox(a);
	BoundingBox bBBox = BoundingBox(b);
	if (dimension == 0) {
		float ca = aBBox.center.x;
		float cb = bBBox.center.x;
		return ca < cb;
	}
	if (dimension == 1) {
		float ca = aBBox.center.y;
		float cb = bBBox.center.y;
		return ca < cb;
	}
	if (dimension == 2) {
		float ca = aBBox.center.z;
		float cb = bBBox.center.z;
		return ca < cb;
	}
	
	return false;
}

TriangleComparer::TriangleComparer()
{
}


TriangleComparer::~TriangleComparer()
{
}
