#include "Object.h"
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

	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			if (fv != 3) {
				perror("non-triangle face found. exiting.");
				exit(-1);
			}
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

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


			//TODO: ADD TRIANGLES TO TRIANGLE ARRAY
			shapes[s].mesh.material_ids[f];
		}
	}
	this->mortonConverter = MortonConverter(minx, miny, minz, maxx, maxy, maxz);
}

Object::~Object()
{
}
