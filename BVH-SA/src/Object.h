#pragma once
#include <string>
#include "MortonConverter.h"
#include "Triangle.h"



// Class that holds parsed .obj data
class Object
{
public:
	MortonConverter mortonConverter;
	Triangle* triangles;
	Object(std::string filePath);
	~Object();
};

