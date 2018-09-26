#pragma once
struct Point; class BVH;
#include "Point.h"
class MortonConverter
{
public:
	float minx, miny, minz, maxx, maxy, maxz;

	unsigned int getMortonCode(Point p);
	Point getPoint(int mortonCode);

	MortonConverter();
	MortonConverter(float minx, float miny, float minz, float maxx, float maxy, float maxz);
	~MortonConverter();
private:
	unsigned int expandBits(unsigned int v);
};

