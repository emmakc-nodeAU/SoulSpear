#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace aie
{
	class Texture;
}
class Camera;
class Shader;

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

class ShaderClassApp : public aie::Application {
public:

	ShaderClassApp();
	virtual ~ShaderClassApp();

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

	Shader *m_shaderProgram;
	//unsigned int m_shader;				// Program(Vertex, Fragment Shaders)
	unsigned int m_projectionViewLoc;

	// CUBE: TEXTURE
	aie::Texture *m_texture;

	// GRID: TEXTURE
	aie::Texture *m_texture1;

	// GRID: TEXTURE - HEIGHT MAP
	//aie::Texture *m_heightmap;

	// camera transforms
	Camera *m_camera;

};