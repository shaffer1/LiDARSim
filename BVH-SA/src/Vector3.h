#pragma once
class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3() : x(), y(), z() {};
	Vector3(float a, float b, float c) : x(a), y(b), z(c) {};
	~Vector3();
};

