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

	//2*n - 1 maximum nodes
	nodes = new BVHNode[2 * o.numTriangles - 1];
	numTriangles = o.numTriangles;
	triangles = new Triangle*[o.numTriangles];
	for (size_t i = 0; i < o.numTriangles; i++) {
		triangles[i] = &o.triangles[i];
	}

	//set the root node
	nodes[0].boundingBox = o.bbox;
	nodes[0].taken = true;
	nodes[0].index = 0;
	nodes[0].isLeaf = false;
	this->numNodes = 1;
}

void BVH::build() {
	size_t leftIndex = 0;
	size_t rightIndex = numTriangles;

	//Probably should turn this into stack-based iterative for performance
	buildRecursive(0, numTriangles, nodes, 0);
}

void BVH::buildRecursive(size_t leftIndex, size_t rightIndex,  BVHNode* current, int depth) {
	current->isLeaf = false;

	//
	if ((rightIndex - leftIndex) <= 10) {
		current->isLeaf = true;
		current->numTriangles = rightIndex - leftIndex;
		current->triangleIndex = leftIndex;
		return;
	}

	//sort triangle subsection by largest dimension
	int largestDimension = current->boundingBox.findLargestDimension();
	TriangleComparer t = TriangleComparer();
	t.dimension = largestDimension;
	std::sort(triangles + leftIndex, triangles + rightIndex, t);
	size_t splitIndex = leftIndex;

	// Find the split index to split the array of triangles. TODO to improve speed: Replace with BINARY search, not linear, or switch to SAH BVH
	while (triangles[splitIndex]->center.getDim(largestDimension) < current->boundingBox.center.getDim(largestDimension) && splitIndex < rightIndex) {
		splitIndex++;
	}

	//if all of triangles are in one side, revert to median split
	if (splitIndex == rightIndex || splitIndex == leftIndex) {
		splitIndex = (leftIndex + rightIndex) / 2;
	}
	
	//set up child nodes
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


//More efficient way to do this?
BoundingBox BVH::calculateBBox(size_t leftIndex, size_t rightIndex) {
	float minx = (std::numeric_limits<float>::max)();
	float miny = (std::numeric_limits<float>::max)();
	float minz = (std::numeric_limits<float>::max)();

	float maxx = std::numeric_limits<float>::lowest();
	float maxy = std::numeric_limits<float>::lowest();
	float maxz = std::numeric_limits<float>::lowest();

	for (size_t i = leftIndex; i < rightIndex; i++) {
		for (int j = 0; j < 3; j++) {
			float vx = triangles[i]->getPoint(j).x;
			float vy = triangles[i]->getPoint(j).y;
			float vz = triangles[i]->getPoint(j).z;
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

//traverses tree and prints the bounding boxes of each one. 
int BVH::printTree(BVHNode* current, int depth)
{
	BoundingBox bbox = current->boundingBox;
	for (int i = 0; i < depth; i++) {
		std::cout << "  ";
	}
	if (current->isLeaf) {
		std::cout << "LEAF ";
	}
	std::cout << "Center: " << bbox.center.x << ", " << bbox.center.y << ", " << bbox.center.z << " rx: " << bbox.rx << " ry: " << bbox.ry << " rz: " << bbox.rz << endl;
	if (current->isLeaf) {
		return 1;
	}

	
	return 1 + printTree(current->leftIndex + nodes, depth + 1) + printTree(current->rightIndex + nodes, depth + 1);
}

int main(int argc, char *argv[]) {
	string filePath(argv[1]);

	BVH* bvh = new BVH();
	bvh->insertBB(filePath);
	bvh->build();
	int numNodes = bvh->printTree(bvh->nodes, 0);
	std::cout << numNodes << endl;

	cin.get();
	return 0;
}