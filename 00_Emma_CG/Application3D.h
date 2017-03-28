#pragma once
#include "Application.h"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "RenderModel.h"

namespace aie
{
	class Texture;
}

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

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	std::vector<OpenGLInfo> m_glInfo;

	// GIZMOS ------------------------------------
	//void CreateGrid();
	//void DestroyGrid();
	//void CreateCube();
	//void DestroyCube();

	void RenderMesh(
		GLMesh *mesh, 
		glm::vec3 &position, 
		glm::vec3 &scale, 
		glm::vec3 materialsColour, 
		aie::Texture *diffuseTexture);

protected:

	// MULTI GEOMETRY RENDER LIBRARY -------------
	using OBJMesh = std::vector<RenderData*>;
	RenderData* m_gridRenderData;
	Shader*		m_gridShader;

	GLMesh m_cube;
	aie::Texture *m_texture;
	aie::Texture *m_whiteTexture;

	// CAMERA ------------------------------------
	Camera*		m_camera;
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
	
	// MODELS ------------------------------------
	OBJMesh		m_bunny;
	Shader*		m_modelShader;

	std::vector<RenderData*> m_SoulSpear;
	Shader		*m_shaderSoulSpear;
	Shader		*m_shaderProgram;
	unsigned int m_projectionViewLoc;

	// LIGHTING
	Shader*		m_lightingShader;
	//unsigned	int modelTemp;
	glm::vec3	m_lightPosition		= glm::vec3(0, 3, 0);
	glm::vec3	m_lightColour		= glm::vec3(1, 0.8, 0.8);
	float		m_ambientStrength	= 0.1f;

};