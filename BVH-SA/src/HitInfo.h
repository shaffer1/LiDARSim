#pragma once
#include "Point.h"
class HitInfo
{
public:
	Point hitPoint;
	bool didHit;

	HitInfo();
	~HitInfo();
};

