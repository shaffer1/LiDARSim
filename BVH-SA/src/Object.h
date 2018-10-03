#pragma once
#include <string>
#include "MortonConverter.h"
#include "Triangle.h"
#include "BoundingBox.h"



// Class that holds parsed .obj data
class Object
{
public:
	MortonConverter mortonConverter;
	Triangle* triangles;
	size_t numTriangles;
	BoundingBox bbox;

	
	Object(std::string filePath);
	~Object();
};

