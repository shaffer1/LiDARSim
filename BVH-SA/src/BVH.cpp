#include "BVH.h"
#include "Object.h"

using namespace std;

int BVH::findNumPoints(string filePath) const {
	ifstream stream(filePath);
	return count(istreambuf_iterator<char>(stream), istreambuf_iterator<char>(), 'v');
}

int BVH::findNumTriangles(string filePath) const {
	ifstream stream(filePath);
	return count(istreambuf_iterator<char>(stream), istreambuf_iterator<char>(), 'f');
}

void BVH::insertBB(string filePath) {
	

}

BoundingBox BVH::assignBB(const Triangle triangle) {
	//Find center
	float xCenter = (max(triangle.p1.x, max(triangle.p2.x, triangle.p3.x)) + min(triangle.p1.x, min(triangle.p2.x, triangle.p3.x))) / 2;
	float yCenter = (max(triangle.p1.y, max(triangle.p2.y, triangle.p3.y)) + min(triangle.p1.y, min(triangle.p2.y, triangle.p3.y))) / 2;
	float zCenter = (max(triangle.p1.z, max(triangle.p2.z, triangle.p3.z)) + min(triangle.p1.z, min(triangle.p2.z, triangle.p3.z))) / 2;

	//Find radius
	float xRadius = max((abs(xCenter - triangle.p1.x)), (max(abs(xCenter - triangle.p2.x), abs(xCenter - triangle.p3.x))));
	float yRadius = max((abs(yCenter - triangle.p1.y)), (max(abs(yCenter - triangle.p2.y), abs(yCenter - triangle.p3.y))));
	float zRadius = max((abs(zCenter - triangle.p1.z)), (max(abs(zCenter - triangle.p2.z), abs(zCenter - triangle.p3.z))));

	return BoundingBox(Point(xCenter, yCenter, zCenter), xRadius, yRadius, zRadius, triangle.idx, true);
}

bool BVH::BBIntersection(const BoundingBox BBa, const BoundingBox BBb) const
{
	if ((abs(BBa.center.x - BBb.center.x) > (BBa.rx + BBb.rx)) ||
		(abs(BBa.center.y - BBb.center.y) > (BBa.ry + BBb.ry)) ||
		(abs(BBa.center.z - BBb.center.z) > (BBa.rz + BBb.rz))) return false;
	return true;
}

int main(int argc, char *argv[]) {
	string filePath(argv[1]);

	BVH* bvh = new BVH();
	Object o(filePath);

	return 0;
}