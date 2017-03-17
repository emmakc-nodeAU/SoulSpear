#pragma once
//https://webrtcweb.com/screen?s=MelbAIE
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
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