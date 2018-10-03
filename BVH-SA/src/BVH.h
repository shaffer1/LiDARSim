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
#include "BVHNode.h"


//Bounding Volume Hierarchy (BVH)
class BVH {
public:
	MortonConverter mortonConverter;
	BVHNode* nodes;			//array containing all bvh nodes
	Triangle* triangles;	//array containing pointers to triangles
	size_t numNodes;
	size_t numTriangles;
	size_t maxCapacity;
	BVH() {};
	BVH(int maxLevels, int maxItemsPerBin);

	int findNumPoints(const std::string filePath) const;
	int findNumTriangles(const std::string filePath) const;
	void insertBB(const std::string filePath);
	void buildRecursive(size_t leftIndex, size_t rightIndex, BVHNode * current, int depth);
	BoundingBox calculateBBox(size_t leftIndex, size_t rightIndex);
	BoundingBox assignBB(const Triangle & triangle);
	bool BBIntersection(BoundingBox BBa, BoundingBox BBb) const;

	//void insert(const BoundingBox& b, const T& item);
	void build();
	//const T* getBinContents(int binIdx); 
	//// find bin that contains the point
	//int findBin(const double* pt) const;
	//// finds the closest bins to a given point, pt
	//void findClosestBins(const double* pt, int*& bins, int& numBins) const;
	//// find intersecting bins with a supplied geometric primitive ( note we can template on the first argument)
	//void findIntersectingBins(const BoundingBox& box, int*& bins, int& numBins) const; 

};

#endif