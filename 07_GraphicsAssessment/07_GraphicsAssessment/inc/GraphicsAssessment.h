#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

namespace aie
{
	class Texture;
}

class Quaternions;
class RenderData;
class Shader;
class Camera;
class ParticleEmitter;
class GPUparticleEmitter;
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

class GraphicsAssessment : public aie::Application {

public:
	GraphicsAssessment();
	virtual ~GraphicsAssessment();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void RenderMesh(
		GLMesh *mesh, 
		glm::vec3 &position, 
		glm::vec3 &scale, 
		glm::vec3 materialsColour, 
		aie::Texture *diffuseTexture);

	void CreateCube();
	void DestroyCube();

	void CreateGrid();
	void DestroyGrid();

	void LoadShader();
	void UnloadShader();

protected:

	// CAMERA
	Camera *m_camera;

	Shader *m_shaderProgram;			// Lighting
	unsigned int m_projectionViewLoc;

	// SOULSPEAR-------------------------------
		// Object
	std::vector<RenderData*> m_SoulSpear;
		// Shader
	Shader *m_shaderSoulSpear;
		// Texture
	aie::Texture *m_SoulSpearDiffuse;
	aie::Texture *m_SoulSpearNormal;
	aie::Texture *m_SoulSpearSpecular;

	// PARTICLES---------------------------------
	Quaternions*		Cube1;
	Quaternions*		Cube2;
	Quaternions*		Cube3;

	ParticleEmitter*	m_emitter1;
	ParticleEmitter*	m_emitter2;
	ParticleEmitter*	m_emitter3;
	Shader*				m_shaderParticles;

	// GPU PARTICLES ----------------------------
	GPUparticleEmitter* e_gpuEmitter1;
	GPUparticleEmitter* e_gpuEmitter2;
	GPUparticleEmitter* e_gpuEmitter3;

	// CUBE--------------------------------------
	GLMesh m_cube;				 // Object
	aie::Texture *m_whiteTexture;// Texture

	// GRID -------------------------------------
	GLMesh m_grid;				// Object
	aie::Texture *m_grass;		// Texture
	aie::Texture *m_floormap;	// bitmap

	// LIGHTING
	glm::vec3 m_lightPosition	= glm::vec3(0, 3, 0);
	glm::vec3 m_lightColour		= glm::vec3(1, 0.8, 0.8);
	float m_ambientStrength		= 0.1f;

	// POST PROCESSING
	PostProcessing* m_postprocessing;
	Shader* m_shaderPostProcessing;
	void soulSpear();
};