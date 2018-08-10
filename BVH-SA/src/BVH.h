#pragma once
#ifndef BVH_H
#define BVH_H

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

//3D Point Representation
struct Point {
	int idx;					//idx of point in given obj file (index begins at 1)
	float x, y, z;

	Point() {};
	Point(float x_arg, float y_arg, float z_arg, int idx_arg) : x(x_arg), y(y_arg), z(z_arg), idx(idx_arg) {}
	Point(float x_arg, float y_arg, float z_arg) : x(x_arg), y(y_arg), z(z_arg) {}
};

//Triangle Representation
struct Triangle {
	int idx;					//keep track of which triangle in which BB (begins at 1 for consistency)
	Point p1, p2, p3;			//points that make up triangle

	Triangle() {};
	Triangle(Point p1_arg, Point p2_arg, Point p3_arg, int idx_arg) : p1(p1_arg), p2(p2_arg), p3(p3_arg), idx(idx_arg) {}
};

//Axis-Aligned Bounding Box (BB)
	//faces always parallel to axes
	//region R = { (x, y, z) | |c.x-x|<=rx, |c.y-y|<=ry, |c.z-z|<=rz }
struct BoundingBox {
	Point center;				//center point of BB
	float rx, ry, rz;			//radius or halfwidth extents
	int triangleIdx;			//idx of triangle BB contains
	bool isLeaf;				//true if BB is leaf in BVH...false if internal node

	BoundingBox(Point center_arg, float rx_arg, float ry_arg, float rz_arg, int triangleIdx_arg, bool isLeaf_arg) : center(center_arg), rx(rx_arg), ry(ry_arg), rz(rz_arg), triangleIdx(triangleIdx_arg), isLeaf(isLeaf_arg) {}
};

//Bounding Volume Hierarchy (BVH)
class BVH {
public:
	BoundingBox* BB[];			//array containing all bounding boxes

	BVH() {};
	BVH(int maxLevels, int maxItemsPerBin);

	int findNumPoints(std::string filePath);
	int findNumTriangles(std::string filePath);
	void insertBB(std::string filePath);
	BoundingBox assignBB(Triangle triangle);
	bool BBIntersection(BoundingBox BBa, BoundingBox BBb);

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