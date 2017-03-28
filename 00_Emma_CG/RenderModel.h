#pragma once
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <vector>

#include "GeometryHelper.h"

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_faceCount;
};

struct OBJVertex
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class RenderModel
{
public:
	static std::vector<RenderData*> LoadOBJFromDisk(std::string path);
protected:
private:
};