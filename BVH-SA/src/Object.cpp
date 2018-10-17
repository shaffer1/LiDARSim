#include "Object.h"
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
Object::Object(std::string filePath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str());
	if (!ret) {
		perror("Object Parsing Failed");
		exit(-1);
	}

	//while parsing this file, calculate the min and max x,y,z values for standardizing values for morton codes
	tinyobj::real_t minx = std::numeric_limits<tinyobj::real_t>::max();
	tinyobj::real_t miny = std::numeric_limits<tinyobj::real_t>::max();
	tinyobj::real_t minz = std::numeric_limits<tinyobj::real_t>::max();

	tinyobj::real_t maxx = std::numeric_limits<tinyobj::real_t>::lowest();
	tinyobj::real_t maxy = std::numeric_limits<tinyobj::real_t>::lowest();
	tinyobj::real_t maxz = std::numeric_limits<tinyobj::real_t>::lowest();

	//count number of triangles in .obj

	for (size_t s = 0; s < shapes.size(); s++) {
		numTriangles += shapes[s].mesh.num_face_vertices.size();
	}
	triangles = new Triangle[numTriangles];

	//indexing starts at one
	int triangles_array_index = 1;
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;

		//allocate some memory to temporarily store point values for each triangle
		Point p[3];

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			if (fv != 3) {
				perror("non-triangle face found. exiting.");
				exit(-1);
			}
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				int idx = shapes[s].mesh.indices[index_offset + v].vertex_index;
				tinyobj::real_t vx = attrib.vertices[3 * idx + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx + 2];

				p[v].idx = idx;
				p[v].x = vx;
				p[v].y = vy;
				p[v].z = vz;

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
			index_offset += fv;
			triangles[triangles_array_index - 1] = Triangle(p[0], p[1], p[2], triangles_array_index);

			triangles_array_index++;
		}
	}
	Point center = Point((maxx + minx) / 2.0f, (maxy + miny) / 2.0f, (maxz + minz) / 2.0f);
	float rx = (maxx - minx) / 2.0f;
	float ry = (maxy - miny) / 2.0f;
	float rz = (maxz - minz) / 2.0f;
	//this->bbox = BoundingBox(center, rx, ry, rz, -1, false);

	numTriangles = triangles_array_index - 1;
	//for (int i = 0; i < num_triangles; i++) {
	//	Triangle t = triangles[i];
	//	std::cout << t.idx << ": " << "(" << t.p1.idx << ", " << t.p2.idx << ", " << t.p3.idx << ")\n";
	//}
	this->mortonConverter = MortonConverter(minx, miny, minz, maxx, maxy, maxz);
}

Object::~Object()
{
}
