#include "MidpointComparer.h"


MidpointComparer::MidpointComparer(int d, double m) : dim(d), midpoint(m)
{
}

bool MidpointComparer::operator()(const BVHObjectInfo & a) const
{
	return a.center.getDim(dim) < midpoint;
}

MidpointComparer::~MidpointComparer()
{

}
