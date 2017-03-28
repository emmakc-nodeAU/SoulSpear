#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <GLFW\glfw3.h>

#include <vector>

namespace aie
{
	class Texture;
}
class RenderData;
class Shader;
class Camera_AIE_ClassicModel;
class ParticleEmitter;
class PostProcessing;

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

class TyphoonLabs : public aie::Application {

public:
	TyphoonLabs();
	virtual ~TyphoonLabs();

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

	// CAMERA
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	GLMesh m_cube;							// Platforms
	GLMesh m_grid;							// Floor
	GLMesh m_skybox;						// Skybox

	// SOULSPEAR
	std::vector<RenderData*> m_SoulSpear;
	// SOULSPEAR - Shader
	Shader *m_shaderSoulSpear;

	// SOULSPEAR: TEXTURE
	aie::Texture *m_SoulSpearDiffuse;
	aie::Texture *m_SoulSpearNormal;
	aie::Texture *m_SoulSpearSpecular;

	Shader *m_shaderProgram;
	//unsigned int m_shader;				// Program(Vertex, Fragment Shaders)
	unsigned int m_projectionViewLoc;

	// PARTICLES
	ParticleEmitter *m_emitter;
	Shader*	m_shaderParticles;
	// CUBE: TEXTURE
	aie::Texture *m_texture;

	// GRID: TEXTURE
	aie::Texture *m_whiteTexture;

	// SPHERE: TEXTURE - SKYBOX
	//aie::Texture *m_stormydays_bk;
	//aie::Texture *m_stormydays_dn;
	//aie::Texture *m_stormydays_ft;
	//aie::Texture *m_stormydays_lf;
	//aie::Texture *m_stormydays_rt;
	//aie::Texture *m_stormydays_up;

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
	Camera_AIE_ClassicModel *m_camera;

	// POST PROCESSING
	PostProcessing* m_postprocessing;
	Shader* m_shaderPostProcessing;
	glm::mat4 projectionView = m_projectionMatrix * m_viewMatrix;
	void soulSpear();
};