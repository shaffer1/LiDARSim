#pragma once
#ifndef BVH_H
#define BVH_H

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Point.h"
#include "Triangle.h"
#include "MortonConverter.h"
#include "BoundingBox.h"
#include "LinearBVHNode.h"
#include "BVHBuildNode.h"
#include "BVHObjectInfo.h"
#include "BVHNode.h"


//Bounding Volume Hierarchy (BVH)
class BVHAccelerator{
public:
	int max_objs_per_leaf;
	int total_nodes;
	BVHBuildNode * recursive_build(BVHObjectInfo * build_data, int build_data_size, int start, int end, int * total_nodes, Triangle ** ordered_objs, int & orderedObjsSize);
	int flatten_bvh(BVHBuildNode* node, int * offset);
	Point findClosestPoint(const Point & p) const;
	float intersectPoint(const Point & p, Point * approximateClosestPoint) const;
	Point intersectSphere(const Sphere& s, Point current) const;
	void printTree(LinearBVHNode* current, int depth) const;
	LinearBVHNode* nodes;
	Triangle** objs;
	int objsSize;
	
	//bool hit(const Ray & r, double & min_t, HitInfo & hit_info) const;

	BVHAccelerator(Triangle** objs, int objsSize, int max_objs_per_leaf);
	BVHAccelerator();
	~BVHAccelerator();

};

#endif