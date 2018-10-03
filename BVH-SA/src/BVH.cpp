#include "BVH.h"
#include "Object.h"
#include "TriangleComparer.h"

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
	Object o = Object(filePath);
	//BoundingBox* b = new BoundingBox[o.num_triangles];
	//BoundingBox worldBoundingBox = o.bbox;
	//for (unsigned int i = 0; i < o.num_triangles; i++) {
	//	b[i] = assignBB(o.triangles[i]);
	//}
	nodes = new BVHNode[2 * o.numTriangles - 1];
	numTriangles = o.numTriangles;
	triangles = o.triangles;
	nodes[0].boundingBox = o.bbox;
	nodes[0].taken = true;
	nodes[0].index = 0;
	this->numNodes = 1;
	
}

void BVH::build() {
	//set starting indices
	size_t leftIndex = 0;
	size_t rightIndex = numTriangles;

	//start the recursive build on the first node
	buildRecursive(0, numTriangles, nodes, 0);
}

void BVH::buildRecursive(size_t leftIndex, size_t rightIndex,  BVHNode* current, int depth) {
	if ((rightIndex - leftIndex) <= 5) {
		current->isLeaf = true;
		current->numTriangles = rightIndex - leftIndex;
		current->triangleIndex = leftIndex;
		return;
	}

	int largestDimension = current->boundingBox.findLargestDimension();
	TriangleComparer t = TriangleComparer();
	t.dimension = largestDimension;

	// TODO to improve speed: Replace with pointer sorting instead of sorting actual triangles (much less overhead from swapping)
	std::sort(triangles + leftIndex, triangles + rightIndex, t);
	size_t splitIndex = leftIndex;

	// TODO to improve speed: Replace with BINARY search, not linear, or switch to SAH BVH
	while (triangles[splitIndex].center.getDim(largestDimension) < current->boundingBox.center.getDim(largestDimension) && splitIndex < rightIndex) {
		splitIndex++;
	}
	if (splitIndex == rightIndex || splitIndex == leftIndex) {
		splitIndex = (leftIndex + rightIndex) / 2;
	}
	
	BVHNode* left = nodes + numNodes;
	BVHNode* right = nodes + numNodes + 1;
	left->taken = true;

	// TODO More efficient bounding box calculation possible?
	left->boundingBox = calculateBBox(leftIndex, splitIndex);
	left->index = numNodes;
	current->leftIndex = numNodes;
	current->rightIndex = numNodes + 1;
	right->taken = true;
	right->boundingBox = calculateBBox(splitIndex, rightIndex);
	right->index = numNodes + 1;
	numNodes += 2;
	
	buildRecursive(leftIndex, splitIndex, left, depth + 1);
	buildRecursive(splitIndex, rightIndex, right, depth + 1);
}

BoundingBox BVH::calculateBBox(size_t leftIndex, size_t rightIndex) {
	float minx = std::numeric_limits<float>::max();
	float miny = std::numeric_limits<float>::max();
	float minz = std::numeric_limits<float>::max();

	float maxx = std::numeric_limits<float>::lowest();
	float maxy = std::numeric_limits<float>::lowest();
	float maxz = std::numeric_limits<float>::lowest();

	for (size_t i = leftIndex; i < rightIndex; i++) {
		float vx = min(min(triangles[i].p1.x, triangles[i].p2.x), triangles[i].p3.x);
		float vy = min(min(triangles[i].p1.y, triangles[i].p2.y), triangles[i].p3.y);
		float vz = min(min(triangles[i].p1.z, triangles[i].p2.z), triangles[i].p3.z);

		if (minx > vx) {
			minx = vx;
		}
		if (miny > vy) {
			miny = vy;
		}
		if (minz > vz) {
			minz = vz;
		}
		if (maxx < vx) {
			maxx = vx;
		}
		if (maxy < vy) {
			maxy = vy;
		}
		if (maxz < vz) {
			maxz = vz;
		}
	}
	Point center = Point((maxx + minx) / 2.0f, (maxy + miny) / 2.0f, (maxz + minz) / 2.0f);
	float rx = (maxx - minx) / 2.0f;
	float ry = (maxy - miny) / 2.0f;
	float rz = (maxz - minz) / 2.0f;
	return BoundingBox(center, rx, ry, rz, -1, false);
}

BoundingBox BVH::assignBB(const Triangle & triangle) {
	//Find center
	float xCenter = (max(triangle.p1.x, max(triangle.p2.x, triangle.p3.x)) + min(triangle.p1.x, min(triangle.p2.x, triangle.p3.x))) / 2.0f;
	float yCenter = (max(triangle.p1.y, max(triangle.p2.y, triangle.p3.y)) + min(triangle.p1.y, min(triangle.p2.y, triangle.p3.y))) / 2.0f;
	float zCenter = (max(triangle.p1.z, max(triangle.p2.z, triangle.p3.z)) + min(triangle.p1.z, min(triangle.p2.z, triangle.p3.z))) / 2.0f;

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
	bvh->insertBB(filePath);
	bvh->build();

	return 0;
}