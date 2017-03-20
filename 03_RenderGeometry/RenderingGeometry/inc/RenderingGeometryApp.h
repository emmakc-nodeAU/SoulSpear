#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

class Camera;

struct Vertex
{
	glm::vec4 position;
	glm::vec4 colour;

	// Describe structure of vertex
	static void SetupVertexAttribPointers();
};

class RenderingGeometryApp : public aie::Application {
public:

	RenderingGeometryApp();
	virtual ~RenderingGeometryApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void CreateGeometry();
	void DestroyGeometry();

	void LoadShader();
	void UnloadShader();

protected:

	unsigned int m_vertCount;
	unsigned int m_indicesCount;

	// BUFFERS FOR OpenGL
	unsigned int m_vao;	// Holds information about Vertex and Binds VBO/IBO
	unsigned int m_vbo;	// Holds vertices
	unsigned int m_ibo;	// Holds indices

	unsigned int m_shader;
	unsigned int m_projectionViewLoc;

	// camera transforms
	Camera *m_camera;

};