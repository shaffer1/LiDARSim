#include "BVH.h"
#include "Object.h"
#include "TriangleComparer.h"


#include "BVH.h"
#include "MidpointComparer.h"
#include <iostream>




BVHBuildNode * BVHAccelerator::recursive_build(BVHObjectInfo* build_data, int build_data_size, int start, int end, int * total_nodes, Triangle** ordered_objs, int & orderedObjsSize)
{
	(*total_nodes)++;
	BVHBuildNode *node = new BVHBuildNode();

	BoundingBox bbox = build_data[start].bbox;
	for (int i = start; i < end; i++) {
		bbox = bbox.union_boxes(build_data[i].bbox);
	}

	int num_objs = end - start;
	if (num_objs <= max_objs_per_leaf) {
		int obj_offset = orderedObjsSize;
		for (int i = start; i < end; i++) {
			int obj_id = build_data[i].id;
			ordered_objs[orderedObjsSize] = objs[obj_id];
			orderedObjsSize++;
		}
		node->init_leaf(obj_offset, num_objs, bbox);
	}
	else {
		BoundingBox centroidBounds = BoundingBox();
		for (int i = start; i < end; i++) {
			centroidBounds = centroidBounds.union_boxes(build_data[i].center);
		}
		int dim = centroidBounds.getMaximumDimension();
		if (centroidBounds.max.getDim(dim) == centroidBounds.min.getDim(dim)) {
			int obj_offset = orderedObjsSize;
			for (int i = start; i < end; i++) {
				int obj_id = build_data[i].id;
				ordered_objs[orderedObjsSize] = objs[obj_id];
				orderedObjsSize++;
			}
			node->init_leaf(obj_offset, num_objs, bbox);
			return node;
		}

		double mid = .5 * (centroidBounds.min.getDim(dim) + centroidBounds.max.getDim(dim));
		BVHObjectInfo* mid_info = std::partition(&build_data[start], &build_data[end - 1] + 1, MidpointComparer(dim, mid));
		int mid_index = mid_info - &build_data[0];
		node->init_interior(dim, recursive_build(build_data, build_data_size, start, mid_index, total_nodes, ordered_objs, orderedObjsSize), recursive_build(build_data, build_data_size, mid_index, end, total_nodes, ordered_objs, orderedObjsSize));
	}
	return node;
}

//bool BVHAccelerator::hit(const Ray & r, double & min_t, HitInfo & hit_info) const
//{
//	if (total_nodes == 0) {
//		return false;
//	}
//	bool hit = false;
//	Direction inverse_direction = 1. / r.direction;
//	int negative_dir[3] = { inverse_direction.x < 0, inverse_direction.y < 0, inverse_direction.z < 0 };
//	int todo_offset = 0, node_num = 0;
//	int todo[64];
//
//	double curr_t = MAX_DOUBLE;
//	double minimum_t = MAX_DOUBLE;
//	RGBColor curr = RGBColor();
//	Direction curr_normal = Direction();
//	//std::cout << "entering loop" << std::endl;
//	while (true) {
//		const LinearBVHNode *node = &nodes[node_num];
//		//std::cout << (int)node->num_objs << std::endl;
//		if (node->bbox.hit(r, min_t, hit_info)) {
//			if (node->num_objs > 0) {
//
//				//std::cout << "checking children" << std::endl;
//				for (int i = 0; i < node->num_objs; i++) {
//					if (objs[node->obj_offset + i]->hit(r, curr_t, hit_info) && curr_t < minimum_t) {
//						minimum_t = curr_t;
//						curr = hit_info.color;
//						curr_normal = hit_info.normal;
//						hit = true;
//					}
//				}
//				if (todo_offset == 0) break;
//				node_num = todo[--todo_offset];
//			}
//			else {
//
//				if (negative_dir[node->axis]) {
//					//std::cout << "traversing child1" << std::endl;
//					todo[todo_offset++] = node_num + 1;
//					node_num = node->child_offset;
//				}
//				else {
//					//std::cout << "traversing child2" << std::endl;
//					todo[todo_offset++] = node->child_offset;
//					node_num = node_num + 1;
//				}
//			}
//		}
//		else {
//			if (todo_offset == 0) break;
//			node_num = todo[--todo_offset];
//		}
//	}
//	hit_info.did_hit = hit;
//	hit_info.hit_point = r.origin + minimum_t * r.direction;
//	hit_info.color = curr;
//	hit_info.normal = curr_normal;
//	min_t = minimum_t;
//	return hit;
//}

int BVHAccelerator::flatten_bvh(BVHBuildNode * node, int * offset)
{
	LinearBVHNode* linear_node = &nodes[*offset];
	linear_node->bbox = node->bounds;
	int local_offset = *offset;
	*offset = *offset + 1;

	//if node is a child
	if (node->num_objs > 0) {
		linear_node->obj_offset = node->first_obj_offset;
		linear_node->num_objs = node->num_objs;
	}
	else {
		linear_node->axis = node->split_axis;
		linear_node->num_objs = 0;
		flatten_bvh(node->children[0], offset);
		linear_node->child_offset = flatten_bvh(node->children[1], offset);
	}
	return local_offset;
}


BVHAccelerator::BVHAccelerator(Triangle** objs, int objsSize, int max_objs_per_leaf) : max_objs_per_leaf(max_objs_per_leaf)
{
	BVHObjectInfo* build_data = (BVHObjectInfo*)malloc(objsSize * sizeof(BVHObjectInfo));
	for (int i = 0; i < objsSize; i++) {
		BoundingBox bbox = objs[i]->getBBox();
		build_data[i] = BVHObjectInfo(i, bbox);
	}
	this->objs = objs;
	int total_nodes = 0;
	Triangle** ordered_objs = (Triangle**)malloc(objsSize * sizeof(Triangle*));
	std::cout << "Num triangles: " << objsSize << std::endl;
	int ordered_objs_size = 0;
	BVHBuildNode* root = recursive_build(build_data, objsSize, 0, objsSize, &total_nodes, ordered_objs, ordered_objs_size);
	nodes = new LinearBVHNode[total_nodes];
	this->total_nodes = total_nodes;

	int offset = 0;
	flatten_bvh(root, &offset);
	free(objs);
	this->objs = ordered_objs;

}

BVHAccelerator::BVHAccelerator()
{
}

BVHAccelerator::~BVHAccelerator()
{
}





//using namespace std;
//
//int BVH::findNumPoints(string filePath) const {
//	ifstream stream(filePath);
//	return count(istreambuf_iterator<char>(stream), istreambuf_iterator<char>(), 'v');
//}
//
//int BVH::findNumTriangles(string filePath) const {
//	ifstream stream(filePath);
//	return count(istreambuf_iterator<char>(stream), istreambuf_iterator<char>(), 'f');
//}
//
//void BVH::insertBB(string filePath) {
//	Object o = Object(filePath);
//
//	//2*n - 1 maximum nodes
//	nodes = new BVHNode[2 * o.numTriangles - 1];
//	numTriangles = o.numTriangles;
//	triangles = new Triangle*[o.numTriangles];
//	for (size_t i = 0; i < o.numTriangles; i++) {
//		triangles[i] = &o.triangles[i];
//	}
//
//	//set the root node
//	nodes[0].boundingBox = o.bbox;
//	nodes[0].taken = true;
//	nodes[0].index = 0;
//	nodes[0].isLeaf = false;
//	this->numNodes = 1;
//}
//
//void BVH::build() {
//	size_t leftIndex = 0;
//	size_t rightIndex = numTriangles;
//
//	//Probably should turn this into stack-based iterative for performance
//	buildRecursive(0, numTriangles, nodes, 0);
//}
//
//void BVH::buildRecursive(size_t leftIndex, size_t rightIndex,  BVHNode* current, int depth) {
//	current->isLeaf = false;
//
//	//
//	if ((rightIndex - leftIndex) <= 10) {
//		current->isLeaf = true;
//		current->numTriangles = rightIndex - leftIndex;
//		current->triangleIndex = leftIndex;
//		return;
//	}
//
//	//sort triangle subsection by largest dimension
//	int largestDimension = current->boundingBox.findLargestDimension();
//	TriangleComparer t = TriangleComparer();
//	t.dimension = largestDimension;
//	std::sort(triangles + leftIndex, triangles + rightIndex, t);
//	size_t splitIndex = leftIndex;
//
//	// Find the split index to split the array of triangles. TODO to improve speed: Replace with BINARY search, not linear, or switch to SAH BVH
//	while (triangles[splitIndex]->center.getDim(largestDimension) < current->boundingBox.center.getDim(largestDimension) && splitIndex < rightIndex) {
//		splitIndex++;
//	}
//
//	//if all of triangles are in one side, revert to median split
//	if (splitIndex == rightIndex || splitIndex == leftIndex) {
//		splitIndex = (leftIndex + rightIndex) / 2;
//	}
//	
//	//set up child nodes
//	BVHNode* left = nodes + numNodes;
//	BVHNode* right = nodes + numNodes + 1;
//	left->taken = true;
//
//	// TODO More efficient bounding box calculation possible?
//	left->boundingBox = calculateBBox(leftIndex, splitIndex);
//	left->index = numNodes;
//	current->leftIndex = numNodes;
//	current->rightIndex = numNodes + 1;
//	right->taken = true;
//	right->boundingBox = calculateBBox(splitIndex, rightIndex);
//	right->index = numNodes + 1;
//	numNodes += 2;
//	
//	buildRecursive(leftIndex, splitIndex, left, depth + 1);
//	buildRecursive(splitIndex, rightIndex, right, depth + 1);
//}
//
//
////More efficient way to do this?
//BoundingBox BVH::calculateBBox(size_t leftIndex, size_t rightIndex) {
//	float minx = (std::numeric_limits<float>::max)();
//	float miny = (std::numeric_limits<float>::max)();
//	float minz = (std::numeric_limits<float>::max)();
//
//	float maxx = std::numeric_limits<float>::lowest();
//	float maxy = std::numeric_limits<float>::lowest();
//	float maxz = std::numeric_limits<float>::lowest();
//
//	for (size_t i = leftIndex; i < rightIndex; i++) {
//		for (int j = 0; j < 3; j++) {
//			float vx = triangles[i]->getPoint(j).x;
//			float vy = triangles[i]->getPoint(j).y;
//			float vz = triangles[i]->getPoint(j).z;
//			if (minx > vx) {
//				minx = vx;
//			}
//			if (miny > vy) {
//				miny = vy;
//			}
//			if (minz > vz) {
//				minz = vz;
//			}
//			if (maxx < vx) {
//				maxx = vx;
//			}
//			if (maxy < vy) {
//				maxy = vy;
//			}
//			if (maxz < vz) {
//				maxz = vz;
//			}
//		}
//	}
//	Point center = Point((maxx + minx) / 2.0f, (maxy + miny) / 2.0f, (maxz + minz) / 2.0f);
//	float rx = (maxx - minx) / 2.0f;
//	float ry = (maxy - miny) / 2.0f;
//	float rz = (maxz - minz) / 2.0f;
//	return BoundingBox(center, rx, ry, rz, -1, false);
//}
//
//BoundingBox BVH::assignBB(const Triangle & triangle) {
//	//Find center
//	float xCenter = (max(triangle.p1.x, max(triangle.p2.x, triangle.p3.x)) + min(triangle.p1.x, min(triangle.p2.x, triangle.p3.x))) / 2.0f;
//	float yCenter = (max(triangle.p1.y, max(triangle.p2.y, triangle.p3.y)) + min(triangle.p1.y, min(triangle.p2.y, triangle.p3.y))) / 2.0f;
//	float zCenter = (max(triangle.p1.z, max(triangle.p2.z, triangle.p3.z)) + min(triangle.p1.z, min(triangle.p2.z, triangle.p3.z))) / 2.0f;
//
//	//Find radius
//	float xRadius = max((abs(xCenter - triangle.p1.x)), (max(abs(xCenter - triangle.p2.x), abs(xCenter - triangle.p3.x))));
//	float yRadius = max((abs(yCenter - triangle.p1.y)), (max(abs(yCenter - triangle.p2.y), abs(yCenter - triangle.p3.y))));
//	float zRadius = max((abs(zCenter - triangle.p1.z)), (max(abs(zCenter - triangle.p2.z), abs(zCenter - triangle.p3.z))));
//
//	return BoundingBox(Point(xCenter, yCenter, zCenter), xRadius, yRadius, zRadius, triangle.idx, true);
//}
//
//bool BVH::BBIntersection(const BoundingBox BBa, const BoundingBox BBb) const
//{
//	if ((abs(BBa.center.x - BBb.center.x) > (BBa.rx + BBb.rx)) ||
//		(abs(BBa.center.y - BBb.center.y) > (BBa.ry + BBb.ry)) ||
//		(abs(BBa.center.z - BBb.center.z) > (BBa.rz + BBb.rz))) return false;
//	return true;
//}
//
//traverses tree and prints the bounding boxes of each one. 
void BVHAccelerator::printTree(LinearBVHNode* current, int depth) const
{
	BoundingBox bbox = current->bbox;
	std::cout << "Min: " << bbox.min.x << ", " << bbox.min.y << ", " << bbox.min.z << " Max: " << bbox.max.x << " , " << bbox.max.y << " , " << bbox.max.z << endl;
	if (current->num_objs > 0) {
		return;
	}

	if (current->obj_offset > 100000) {
		return;
	}
	
	printTree(current + 1, depth + 1);
	printTree(current + current->child_offset, depth + 1);
}

int main(int argc, char *argv[]) {
	string filePath(argv[1]);
	Object o = Object(filePath);
	Triangle ** triangles = new Triangle*[o.numTriangles];
	for (int i = 0; i < o.numTriangles; i++) {
		triangles[i] = &o.triangles[i];
	}

	BVHAccelerator* bvh = new BVHAccelerator(triangles, o.numTriangles, 1);

	bvh->printTree(bvh->nodes, 0);

	cin.get();
	return 0;
}