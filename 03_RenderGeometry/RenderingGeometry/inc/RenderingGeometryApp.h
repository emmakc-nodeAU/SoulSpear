#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace aie
{
	class Texture;
}
class Camera;

struct Vertex
{
	glm::vec4 position;
	glm::vec4 colour;
	glm::vec2 uv;

	// Describe structure of vertex
	static void SetupVertexAttribPointers();
};

struct GLMesh
{
	unsigned int vertCount;
	unsigned int indicesCount;

	unsigned int vao;	// Holds information about Vertex and Binds VBO/IBO
	unsigned int vbo;	// Holds vertices
	unsigned int ibo;	// Holds indices
};

class RenderingGeometryApp : public aie::Application {
public:

	RenderingGeometryApp();
	virtual ~RenderingGeometryApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void CreateCube();
	void DestroyCube();

	void CreateGrid();
	void DestroyGrid();

	void LoadShader();
	void UnloadShader();

protected:

	GLMesh m_cube;
	GLMesh m_grid;

	unsigned int m_shader;
	unsigned int m_projectionViewLoc;

	// CUBE: TEXTURE
	aie::Texture *m_texture;

	// GRID: TEXTURE
	aie::Texture *m_texture1;

	// camera transforms
	Camera *m_camera;

};