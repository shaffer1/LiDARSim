#pragma once
#include "BVHObjectInfo.h"
class MidpointComparer
{
public:
	int dim;
	double midpoint;
	MidpointComparer(int d, double m);
	bool operator()(const BVHObjectInfo &a) const;
	~MidpointComparer();
};

