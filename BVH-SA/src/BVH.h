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


//Axis-Aligned Bounding Box (BB)
	//faces always parallel to axes
	//region R = { (x, y, z) | |c.x-x|<=rx, |c.y-y|<=ry, |c.z-z|<=rz }
struct BoundingBox {
	Point center;				//center point of BB
	unsigned int mortonCode;	//morton code of center point
	float rx, ry, rz;			//radius or halfwidth extents
	int triangleIdx;			//idx of triangle BB contains
	bool isLeaf;				//true if BB is leaf in BVH...false if internal node

	BoundingBox(Point center_arg, float rx_arg, float ry_arg, float rz_arg, int triangleIdx_arg, bool isLeaf_arg) : center(center_arg), rx(rx_arg), ry(ry_arg), rz(rz_arg), triangleIdx(triangleIdx_arg), isLeaf(isLeaf_arg) {}
};

//Bounding Volume Hierarchy (BVH)
class BVH {
public:
	MortonConverter mortonConverter;
	BoundingBox* BB[];			//array containing all bounding boxes

	BVH() {};
	BVH(int maxLevels, int maxItemsPerBin);

	int findNumPoints(const std::string filePath) const;
	int findNumTriangles(const std::string filePath) const;
	void insertBB(const std::string filePath);
	BoundingBox assignBB(const Triangle triangle);
	bool BBIntersection(BoundingBox BBa, BoundingBox BBb) const;

	//void insert(const BoundingBox& b, const T& item);
	//void build();
	//const T* getBinContents(int binIdx); 
	//// find bin that contains the point
	//int findBin(const double* pt) const;
	//// finds the closest bins to a given point, pt
	//void findClosestBins(const double* pt, int*& bins, int& numBins) const;
	//// find intersecting bins with a supplied geometric primitive ( note we can template on the first argument)
	//void findIntersectingBins(const BoundingBox& box, int*& bins, int& numBins) const; 

};

#endif