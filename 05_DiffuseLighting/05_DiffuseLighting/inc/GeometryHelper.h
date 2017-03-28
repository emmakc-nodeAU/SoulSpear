#pragma once
#include <string>
#include <vector>
class RenderData;
namespace GeometryHelper
{
	struct OBJVertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	};
	std::vector<RenderData*> loadObjFromFile(std::string filepath);
};