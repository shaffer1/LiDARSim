#include "MortonConverter.h"



//morton magic courtesy of https://devblogs.nvidia.com/thinking-parallel-part-iii-tree-construction-gpu/
unsigned int MortonConverter::getMortonCode(Point p)
{
	unsigned int x = (unsigned int)(((p.x - minx) / (maxx - minx)) * (1024.0f));
	unsigned int y = (unsigned int)(((p.y - miny) / (maxy - miny)) * (1024.0f));
	unsigned int z = (unsigned int)(((p.z - minz) / (maxz - minz)) * (1024.0f));

	unsigned int xx = expandBits(x);
	unsigned int yy = expandBits(y);
	unsigned int zz = expandBits(z);

	return xx * 4 + yy * 2 + zz;
}

Point MortonConverter::getPoint(int mortonCode)
{
	return Point();
}

MortonConverter::MortonConverter()
{
}

MortonConverter::MortonConverter(float minx, float miny, float minz, float maxx, float maxy, float maxz) : minx(minx), miny(miny), minz(minz), maxx(maxx), maxy(maxy), maxz(maxz)
{
}

MortonConverter::~MortonConverter()
{
}


unsigned int MortonConverter::expandBits(unsigned int v)
{
	v = (v * 0x00010001u) & 0xFF0000FFu;
	v = (v * 0x00000101u) & 0x0F00F00Fu;
	v = (v * 0x00000011u) & 0xC30C30C3u;
	v = (v * 0x00000005u) & 0x49249249u;
	return v;
}
