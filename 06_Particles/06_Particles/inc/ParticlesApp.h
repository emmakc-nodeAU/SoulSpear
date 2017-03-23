#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace aie
{
	class Texture;
}
class ParticleEmitter;
class Camera;
class Shader;
class RenderData;

struct Vertex
{
	glm::vec4 position;
	glm::vec4 colour;
	glm::vec2 uv;
	glm::vec3 normal;

	// Describe structure of vertex
	static void SetupVertexAttribPointers();
};

struct GLMesh
{
	unsigned int vertCount;
	unsigned int indicesCount;

	unsigned int indicesSizeType;

	unsigned int vao;	// Holds information about Vertex and Binds VBO/IBO
	unsigned int vbo;	// Holds vertices
	unsigned int ibo;	// Holds indices
};

class ParticlesApp : public aie::Application {
public:

	ParticlesApp();
	virtual ~ParticlesApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void RenderMesh(GLMesh *mesh, glm::vec3 &position, glm::vec3 &scale, glm::vec3 materialsColour, aie::Texture *diffuseTexture);

	void CreateCube();
	void DestroyCube();

	void CreateGrid();
	void DestroyGrid();

	void LoadShader();
	void UnloadShader();

protected:

	GLMesh m_cube;							// Platforms
	GLMesh m_grid;							// Floor
	GLMesh m_skybox;						// Skybox

	std::vector<RenderData*> m_SoulSpear;
	Shader *m_shaderSoulSpear;

	Shader *m_shaderProgram;
	//unsigned int m_shader;				// Program(Vertex, Fragment Shaders)
	unsigned int m_projectionViewLoc;

	// PARTICLES
	ParticleEmitter *m_emitter;

	// CUBE: TEXTURE
	aie::Texture *m_texture;

	// GRID: TEXTURE
	aie::Texture *m_whiteTexture;

	// SPHERE: TEXTURE - SKYBOX
	aie::Texture *m_stormydays_bk;
	aie::Texture *m_stormydays_dn;
	aie::Texture *m_stormydays_ft;
	aie::Texture *m_stormydays_lf;
	aie::Texture *m_stormydays_rt;
	aie::Texture *m_stormydays_up;

	// GRID: TEXTURE - BITMAP
	aie::Texture *m_floormap;

	// GRID: TEXTURE - HEIGHT MAP
	//aie::Texture *m_heightmap;


	// LIGHTING
	glm::vec3 m_lightPosition = glm::vec3(0, 3, 0);
	glm::vec3 m_lightColour = glm::vec3(1, 0.8, 0.8);
	float m_ambientStrength = 0.1f;

	// SKYBOX
	//glm::vec3 m_skyboxPosition = glm::vec3(0, 10, 0);
	//glm::vec3 m_skyboxColour = glm::vec3(1, 0.8, 0.8);

	// FLOOR
	//glm::vec3 m_floorPosition = glm::vec3(0, 10, 0);
	//glm::vec3 m_floorColour = glm::vec3(1, 0, 0);

	// camera transforms
	Camera *m_camera;

};