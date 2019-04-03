#pragma once
#include "ColoredPoint.h";
class HitInfo
{
public:
	ColoredPoint hitPoint;
	bool didHit;

	HitInfo();
	~HitInfo();
};

