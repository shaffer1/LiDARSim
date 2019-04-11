#include "BVH.h"
#include "Object.h"
#include "TriangleComparer.h"
#include "MidpointComparer.h"
#include "Constants.h"
#include "bitmap_image.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>
#include <RAJA/RAJA.hpp>

#define DEG2RAD 0.01745329251


std::ostream& operator<<(std::ostream& os, const ColoredPoint& pt)
{
	os << "(x: " << pt.x << ", y: " << pt.y << ", z: " << pt.z << ") (r: " << pt.r << ", g: " << pt.g << ", b: " << pt.b << ")";
	return os;
}

inline float sqrDist(const Point & p1, const Point & p2) {
	return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) + (p2.z - p1.z) * (p2.z - p1.z);
}


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

float BVHAccelerator::intersectPoint(const Point & p, Point * approximateClosestPoint) const
{
	if (total_nodes == 0) {
		return false;
	}
	bool hit = false;
	int todo_offset = 0, node_num = 0;
	int todo[64];
	

	//std::cout << "entering loop" << std::endl;
	while (true) {
		const LinearBVHNode *node = &nodes[node_num];
		//std::cout << (int)node->num_objs << std::endl;
		if (node->num_objs > 0) {
			//std::cout << "checking children" << std::endl;
			hit = true;
			float min_distance = 100000;
			for (int i = 0; i < node->num_objs; i++) {
				float dist = sqrDist(objs[node->obj_offset + i]->p1, p);
				if (dist < min_distance) {
					min_distance = dist;
					*approximateClosestPoint = objs[node->obj_offset + i]->p1;
				}
				dist = sqrDist(objs[node->obj_offset + i]->p2, p);
				if (dist < min_distance) {
					min_distance = dist;
					*approximateClosestPoint = objs[node->obj_offset + i]->p2;
				}
				dist = sqrDist(objs[node->obj_offset + i]->p3, p);
				if (dist < min_distance) {
					min_distance = dist;
					*approximateClosestPoint = objs[node->obj_offset + i]->p3;
				}
			}
			return sqrt(min_distance);
		}
		else {
			Point mid1 = (nodes[node_num + 1].bbox.min + node->bbox.max) / 2.;
			Point mid2 = (nodes[node->child_offset].bbox.min + node->bbox.max) / 2.;
			float dist1 = sqrDist(p, mid1);
			float dist2 = sqrDist(p, mid2);

			if (dist1 < dist2) {
				node_num = node_num + 1;
			}
			else {
				node_num = node->child_offset;
			}
		}
	}
	return -1;
}

 Point BVHAccelerator::intersectSphere(const Sphere & s, Point current) const {
	 if (total_nodes == 0) {
		 return Point();
	 }
	 bool hit = false;
	 int todo_offset = 0, node_num = 0;
	 int todo[64];
	 float currentMinDistance = s.radius * s.radius;

	 //std::cout << "entering loop" << std::endl;
	 while (true) {
		 const LinearBVHNode *node = &nodes[node_num];
		 if (node->bbox.max.x == 4. || node->bbox.max.y == 4. || node->bbox.max.z == 4.) {
			 hit = true;
		 }
		 //std::cout << (int)node->num_objs << std::endl;
		 if (node->bbox.intersect(s)) {
			 if (node->num_objs > 0) {
				 //std::cout << "checking children" << std::endl;
				 hit = true;
				 for (int i = 0; i < node->num_objs; i++) {
					 float dist = sqrDist(objs[node->obj_offset + i]->p1, s.center);
					 if (dist < currentMinDistance) {
						 currentMinDistance = dist;
						 current = objs[node->obj_offset + i]->p1;
					 }
					 dist = sqrDist(objs[node->obj_offset + i]->p2, s.center);
					 if (dist < currentMinDistance) {
						 currentMinDistance = dist;
						 current = objs[node->obj_offset + i]->p2;
					 }
					 dist = sqrDist(objs[node->obj_offset + i]->p3, s.center);
					 if (dist < currentMinDistance) {
						 currentMinDistance = dist;
						 current = objs[node->obj_offset + i]->p3;
					 }
				 }
				 if (todo_offset == 0) break;
				 node_num = todo[--todo_offset];
			 }
			 else {
				 todo[todo_offset++] = node_num + 1;
				 node_num = node->child_offset;
			 }
		 }
		 else {
			 if (todo_offset == 0) break;
			 node_num = todo[--todo_offset];
		 }
	 }
	 return current;
}

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

Point BVHAccelerator::findClosestPoint(const Point & p) const {
	Point closest = Point();
	float sphereRadius = intersectPoint(p, &closest);
	Sphere s = Sphere();
	s.center = p;
	s.radius = sphereRadius;
	closest = intersectSphere(s, closest);
	return closest;
}

bool BVHAccelerator::hit(const Ray & r, double & minT, HitInfo & hitInfo) {
	if (total_nodes == 0) {
		return false;
	}
	bool hit = false;
	double currentT = MAX_DOUBLE;
	double currentMinimumT = MAX_DOUBLE;
	Triangle* currTriangle;
	float color[3];

	int todoOffset = 0; 
	int nodeNum = 0;
	int todo[64];

	Point inverseDirection = r.direction.inverse(); 
	int negativeDir[3] = { inverseDirection.x < 0, inverseDirection.y < 0, inverseDirection.z < 0 };

	while (true) {
		const LinearBVHNode* node = &nodes[nodeNum];
		if (node->bbox.hit(r)) {
			//checking children
			if (node->num_objs > 0) {
				for (int i = 0; i < node->num_objs; i++) {
					Triangle* tri = objs[node->obj_offset + i];
					if (tri->hit(r, currentT, hitInfo) && currentT < currentMinimumT && currentT > 0) {
						currentMinimumT = currentT;
						currTriangle = tri;
						color[0] = tri->p1.r;
						color[1] = tri->p1.g;
						color[2] = tri->p1.b;
						hit = true;
					}
				}
				if (todoOffset == 0) break;
				nodeNum = todo[--todoOffset];
			}
			else {
				if (negativeDir[node->axis]) {
					//traverse child 1 first
					todo[todoOffset++] = nodeNum + 1;
					nodeNum = node->child_offset;
				}
				else {
					//traverse child 2 first
					todo[todoOffset++] = node->child_offset;
					nodeNum = nodeNum + 1;
				}
			}
		}
		else {
			if (todoOffset == 0) break;
			nodeNum = todo[--todoOffset];
		}
	}

	hitInfo.didHit = hit;
	Point p = (r.origin + r.direction * currentMinimumT);
	hitInfo.hitPoint = ColoredPoint(p.x, p.y, p.z, color[0], color[1], color[2]);
	minT = currentMinimumT;

	return hit;
}

float randomFloat() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

Point randomPoint() {
	return Point(randomFloat(), randomFloat(), randomFloat());
}

Point sphericalToCartesian(float r, float polar, float azimuth) {
	Point p = Point();
	polar = DEG2RAD * polar;
	azimuth = DEG2RAD * azimuth;
	p.x = r * cosf(polar) * cosf(azimuth);
	p.z = r * cosf(polar) * sinf(azimuth);
	p.y = r * sinf(polar);
	return p;
}

void launchRays(int numRaysH, int numRaysV, const Point & origin, BVHAccelerator* bvh, bool labeled) {
	float angleDeltaH = 360. / numRaysH;
	float angleDeltaV = 180. / numRaysV;

	Point end;
	Ray r;
	r.origin = origin;
	HitInfo h = HitInfo();
	double minT = MAX_DOUBLE;

	std::vector<vector <Point>> dists = std::vector<vector <Point>>(numRaysV);
	for (int i = 0; i < numRaysV; i++) {
		dists[i] = vector<Point>(numRaysH);
	}

	bool success;
	for (int i = 0; i < numRaysV; i++) {
		for (int j = 0; j < numRaysH; j++) {
			end = origin + sphericalToCartesian(1., -90 + i * angleDeltaV, j*angleDeltaH);
			r.direction = end - origin;
			success = bvh->hit(r, minT, h);
			if (success) {
				if (!labeled) {
					dists[i][j] = Point(minT, minT, minT);
				}
				else {
					dists[i][j] = Point(h.hitPoint.r, h.hitPoint.g, h.hitPoint.b);
				}
				//std::cout << h.hitPoint << std::endl;
			}
			else {
				dists[i][j] = Point(0, 0, 0);
				//std::cout << "No hit" << std::endl;
			}
		}
	}

	float max = -1;

	class Comp {
	public:
		bool operator()(const Point & p1, const Point & p2) {
			return p1.x < p2.x;
		}
	};
	Comp comp;
	if (!labeled) {
		for (auto vec : dists) {
			float smallmax = (*(std::max_element(vec.begin(), vec.end(), comp))).x;
			max = std::max(max, smallmax);
		}

		Point p = Point(1, 1, 1);
		for (auto & vec : dists) {
			for (auto & elem : vec) {
				if (elem.x) {
					elem = p - (elem / max);
				}
			}
		}
	}

	bitmap_image bmp = bitmap_image(numRaysH, numRaysV);

	for (int i = 0; i < dists.size(); i++) {
		for (int j = 0; j < dists[i].size(); j++) {
			Point color = dists[i][j];
			unsigned char char_r;
			unsigned char char_g;
			unsigned char char_b;

			char_r = (unsigned char)(color.x * 255.);
			char_g = (unsigned char)(color.y * 255.);
			char_b = (unsigned char)(color.z * 255.);


			int y = numRaysV - i - 1; //changing because of how bmps are written to

			bmp.set_pixel(j, y, char_r, char_g, char_b);
		}
	}
	bmp.set_pixel(0, numRaysV - 0 - 1, 100, 100, 100);
	bmp.save_image("test.bmp");

}

int main(int argc, char *argv[]) {

	string filePath(argv[1]);
	Object o = Object(filePath);
	Triangle ** triangles = new Triangle*[o.numTriangles];

	for (int i = 0; i < o.numTriangles; i++) {
		triangles[i] = &o.triangles[i];
	}
	std::cout << "Starting BVH build" << std::endl;
	BVHAccelerator* bvh = new BVHAccelerator(triangles, o.numTriangles, 20);
	std::cout << "Ending BVH build" << std::endl;

	//Point orig; 
	//Point d;
	//Ray r = Ray();
	//HitInfo h = HitInfo();
	//double minT = MAX_DOUBLE;
	//bool success;
	auto start = chrono::high_resolution_clock::now();

	launchRays(800, 400, Point(0, 3, 3.5), bvh, false);

	//r.origin = Point(0, 3, -10);
	//r.direction = Point(0, 0, 1);
	

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

	//if (success) std::cout << h.hitPoint << std::endl;
	std::cout << duration.count() << std::endl;
	std::cin.get();


	/*auto start = chrono::high_resolution_clock::now();
	Point* point = new Point(0., 0., 0.);
	RAJA::forall<RAJA::omp_parallel_for_exec>(RAJA::RangeSegment(0, 10000), [=](int i) {
		point->x = randomFloat();
		point->y = randomFloat();
		point->z = randomFloat();
		Point closest = bvh->findClosestPoint(*point);
	});


	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
	std::cout << "RAJA Time: " << duration.count() << std::endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++) {
		point->x = randomFloat();
		point->y = randomFloat();
		point->z = randomFloat();
		Point closest = bvh->findClosestPoint(*point);
	}
	stop = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
	
	std::cout << "Normal Time: " << duration.count() << std::endl;
	std::cin.get();*/
	return 0;
}