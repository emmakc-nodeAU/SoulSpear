#include "GraphicsAssessment.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include "Quaternions.h"
#include "ParticleEmitter.h"
#include "GPUparticleEmitter.h"

#include <Texture.h>
#include "RenderData.h"
#include "FlyCamera.h"
#include "Shader.h"
#include "GeometryHelper.h"

#include <imgui.h>

#include "PostProcessing.h"
#include "SphereBoundingVolume.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsAssessment::GraphicsAssessment() {}

GraphicsAssessment::~GraphicsAssessment() {}

bool GraphicsAssessment::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);		// Grey world space

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// CAMERA
	m_camera = new FlyCamera(m_window, 5.0f);
	m_camera->setPerspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.0f);

	m_camera->lookAt(glm::vec3(30, 30, 30),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	// GRID
	m_grass = new aie::Texture("./textures/grass.png");

	// CUBE
	m_whiteTexture = new aie::Texture("./textures/white.png");
	
	// SHADER
	LoadShader();
	
	// QUATERNION
	Cube1 = new Quaternions;
	Cube1->startup();

	Cube2 = new Quaternions;
	Cube2->startup();

	Cube3 = new Quaternions;
	Cube3->startup();
	
	// GEOMETRY
	CreateCube();
	CreateGrid();

	// SOUL SPEAR
	m_SoulSpear = GeometryHelper::loadObjFromFile("./models/soulspear/soulspear.obj");
	m_shaderSoulSpear = new Shader("./shaders/soulspear.vert", "./shaders/soulspear.frag");
	m_SoulSpearDiffuse = new aie::Texture("./models/soulspear/soulspear_diffuse.tga");	
	m_SoulSpearNormal = new aie::Texture("./models/soulspear/soulspear_normal.tga");
	m_SoulSpearSpecular = new aie::Texture("./models/soulspear/soulspear_specular.tga");
	
	// PARTICLES 1
	m_emitter1 = new ParticleEmitter();
	m_emitter1->initalise(1000, 5000,
	0.1f, 1.0f,
	1, 5,
	1, 0.1f,
	glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));
	// PARTICLES 2
	m_emitter2 = new ParticleEmitter();
	m_emitter2->initalise(1000, 5000,
		0.1f, 1.0f,
		1, 5,
		1, 0.1f,
		glm::vec4(5, 0, 0, 1), glm::vec4(5, 1, 0, 1));
	// PARTICLES 3
	m_emitter3 = new ParticleEmitter();
	m_emitter3->initalise(1000, 5000,
		0.1f, 1.0f,
		1, 5,
		1, 0.1f,
		glm::vec4(10, 0, 0, 1), glm::vec4(10, 1, 0, 1));


	// GPU PARTICLES
	e_gpuEmitter1 = new GPUparticleEmitter();
	e_gpuEmitter1->initalise(100000, 0.01f, 5.0f, 5, 20, 0.001f, 0.01f,
		glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));
	// 2
	e_gpuEmitter2 = new GPUparticleEmitter();
	e_gpuEmitter2->initalise(100000, 0.01f, 5.0f, 5, 20, 0.001f, 0.01f,
		glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));
	// 3
	e_gpuEmitter3 = new GPUparticleEmitter();
	e_gpuEmitter3->initalise(100000, 0.01f, 5.0f, 5, 20, 0.001f, 0.01f,
		glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));

	m_shaderParticles = new Shader("./shaders/Particle.vert", "./shaders/Particle.frag");
	
	// POST PROCESSING
	m_postprocessing = new PostProcessing();
	m_postprocessing->startup();
	m_shaderPostProcessing = new Shader("./shaders/PostProcessing.vert", "./shaders/PostProcessing.frag");
	
	return true;
}

void GraphicsAssessment::shutdown() {
	
	delete m_whiteTexture;
	delete m_grass;

	DestroyGrid();
	DestroyCube();
	UnloadShader();

	delete m_camera;
	Gizmos::destroy();
}

void GraphicsAssessment::update(float deltaTime) {

	float time = getTime();

	// LIGHT Orbits around x axis
	m_lightPosition.x = glm::cos(time) * 5;
	m_lightPosition.z = glm::sin(time) * 5;

	// GUI Controls
	ImGui::SliderFloat("AmbientStrength", &m_ambientStrength, 0.0f, 1.0f);
	ImGui::SliderFloat3("LightColour", &m_lightColour[0], 0.0f, 1.0f);

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// LIGHT - BALL REPRESENTS THE LIGHT
	//	position: m_lightPosition
	//	radius	: 0.1f
	//	size:	16 x 16
	//	fill colour: m_lightColour cast vec4, vec3+, 1 for transparency
	Gizmos::addSphere(m_lightPosition, 0.1f, 16, 16, glm::vec4(m_lightColour, 1.0f));

	// GRID: Structure with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}
	// GRID: add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// PARTICLES
	m_camera->Update(deltaTime);
	Cube1->update(deltaTime);
	m_emitter1->update(deltaTime, m_camera->getTransform());
	m_emitter1->movePosition(Cube1->getPosition());
	// 2
	Cube2->update(deltaTime);
	m_emitter2->update(deltaTime, m_camera->getTransform());
	m_emitter2->movePosition(Cube2->getPosition());
	// 3
	Cube3->update(deltaTime);
	m_emitter3->update(deltaTime, m_camera->getTransform());
	m_emitter3->movePosition(Cube3->getPosition());

	// GPU PARTICLES
	//e_gpuEmitter->movePosition(Cube->getPosition());

	// QUIT on ESC
	aie::Input* input = aie::Input::getInstance();
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsAssessment::draw() {
	// CAMERA
	Gizmos::draw(m_camera->getProjectionView());

	// wipe the screen to the background colour
	clearScreen();

	// Step 1: Before rendering geometry, tell OpenGl to use Shader Program
	glUseProgram(m_shaderProgram->GetProgramID());
	// Step 2: Calculate projection view matrix, pass into shader program
	glUniformMatrix4fv(m_projectionViewLoc, 1, false, &m_camera->getProjectionView()[0][0]);

	// Lighting diffuse shader
	glUniform1fv(glGetUniformLocation(m_shaderProgram->GetProgramID(), "lightAmbientStrength"), 1, &m_ambientStrength);
	glUniform3fv(glGetUniformLocation(m_shaderProgram->GetProgramID(), "lightColour"), 1, &m_lightColour[0]); // Light colour
	glUniform3fv(glGetUniformLocation(m_shaderProgram->GetProgramID(), "lightPos"), 1, &m_lightPosition[0]); // Light Position

	// RENDER CUBE:
	// Position |  ?  | Colour | Texture
	RenderMesh(&m_cube, glm::vec3(-4, 2, 6), glm::vec3(1, 1, 1), glm::vec3(2, 0, 2), m_whiteTexture);		// SCREEN	- LHS
	RenderMesh(&m_cube, glm::vec3(-4, 2, -6), glm::vec3(1, 1, 1), glm::vec3(1, 0, 2), m_whiteTexture);		// SCREEN	- RHS
	RenderMesh(&m_cube, glm::vec3(-2, 2, 4), glm::vec3(1, 1, 1), glm::vec3(0, 2, 1), m_whiteTexture);		// PLATFORM - LHS
	RenderMesh(&m_cube, glm::vec3(-2, 2, -4), glm::vec3(1, 1, 1), glm::vec3(1, 2, 1), m_whiteTexture);		// PLATFORM - RHS
	RenderMesh(&m_cube, glm::vec3(2, 2, 0), glm::vec3(1, 1, 1), glm::vec3(2, 2, 2), m_whiteTexture);	// PLATFORM - CENTRE

	// RENDER GRID:
	RenderMesh(&m_grid, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), m_grass);				// FLOOR
	
	// SHADER: SOULSPEAR
	// 1. Select Shader Program:
	glUseProgram(m_shaderSoulSpear->GetProgramID());
	// 2. Calculate PVM, pass into shader program
//	glm::mat4 projectionView = m_projectionMatrix * m_viewMatrix;
	int loc = glGetUniformLocation(m_shaderSoulSpear->GetProgramID(), "projectionViewWorldMatrix");
	glUniformMatrix4fv(loc, 1, false, &m_camera->getProjectionView()[0][0]);
	
	loc = glGetUniformLocation(m_shaderSoulSpear->GetProgramID(), "modelMatrix");
	glm::mat4 modelMatrix(1);
	glUniformMatrix4fv(loc, 1, false, &modelMatrix[0][0]);

	// RENDER: SOULSPEAR
	for (auto& mesh : m_SoulSpear)
	{
		mesh->Bind();
		glDrawArrays(GL_TRIANGLES, 0, mesh->GetNumberOfIndicies());
	}

	// POST PROCESSING:
	glBindFramebuffer(GL_FRAMEBUFFER, m_postprocessing->m_fbo);
	glViewport(0, 0, 512, 512);
	
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// REFLECTION OBJECTS TO RENDER HERE:
	Gizmos::draw(m_camera->getProjectionView());
	soulSpear();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);
	
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(m_shaderPostProcessing->GetProgramID());
	
	loc = glGetUniformLocation(m_shaderPostProcessing->GetProgramID(), "projectionViewMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_camera->getProjectionView()[0][0]));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_postprocessing->m_fboTexture);
	glUniform1i(glGetUniformLocation(m_shaderPostProcessing->GetProgramID(), "diffuse"), 0);
	
	glBindVertexArray(m_postprocessing->m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	soulSpear();

	// WINDOW RESIZE
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
	//	getWindowWidth() / (float)getWindowHeight(),
	//	0.1f, 1000.0f);	

	// Step 6: Deactivate Shader program

	// PARTICLES
	glUseProgram(m_shaderParticles->GetProgramID());
	loc = glGetUniformLocation(m_shaderParticles->GetProgramID(), "projectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m_camera->getProjectionView()[0][0]);
	//glUniformMatrix4fv(loc, 1, false, &(m_camera->getProjectionView()[0][0]));
	m_emitter1->draw();
	m_emitter2->draw();
	m_emitter3->draw();

	// GPU PARTICLES
	e_gpuEmitter1->draw((float)glfwGetTime(),
		m_camera->getTransform(),
		m_camera->getProjectionView());

	e_gpuEmitter2->draw((float)glfwGetTime(),
		m_camera->getTransform(),
		m_camera->getProjectionView());

	e_gpuEmitter3->draw((float)glfwGetTime(),
		m_camera->getTransform(),
		m_camera->getProjectionView());

	glUseProgram(0);
	Gizmos::draw(m_camera->getProjectionView());
}


void GraphicsAssessment::RenderMesh(GLMesh *mesh, glm::vec3 &position, glm::vec3 &scale, glm::vec3 materialColour, aie::Texture *diffuseTexture)
{
	glm::mat4 model(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		position.x, position.y, position.z, 1
	);

	// Sends Model Matrix
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->GetProgramID(), "model"), 1, false, &model[0][0]);

	// Sends Colour
	glUniform3fv(glGetUniformLocation(m_shaderProgram->GetProgramID(), "diffuseColour"), 1, &materialColour[0]);

	// Texture: GPU texture slot zero(0)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture->getHandle());
	glUniform1i(glGetUniformLocation(m_shaderProgram->GetProgramID(), "texture"), 0);

	// Step 3: Render and Bind VAO mesh
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->indicesCount, mesh->indicesSizeType, 0);
	glBindVertexArray(0);
}


void GraphicsAssessment::CreateCube()
{
	/*
	STEP 1: 
	Specify position, colour for each vert of a cube
	Example: Each face holds 4 verts
	*/
	
	glm::vec4 white		(1.0f, 1.0f, 1.0f, 1.0f);	// Defined colour variable "White"
	glm::vec4 red		(1.0f, 0.0f, 0.0f, 1.0f);	// Defined colour variable "Red"
	glm::vec4 yellow	(1.0f, 1.0f, 0.0f, 1.0f);	// Defined colour variable "Yellow"
	glm::vec4 green		(0.0f, 1.0f, 0.0f, 1.0f);	// Defined colour variable "Green"
	glm::vec4 cyan		(0.0f, 1.0f, 1.0f, 1.0f);	// Defined colour variable "Cyan"
	glm::vec4 blue		(0.0f, 0.0f, 1.0f, 1.0f);	// Defined colour variable "Blue"
	glm::vec4 magenta	(1.0f, 0.0f, 1.0f, 1.0f);	// Defined colour variable "Magenta"
	
	Vertex verts[] = {
	// {POSITION} , {COLOUR}
															// FRONT FACE	- WHITE
	{ { -0.5f,-0.5f, 0.5f, 1.0f },white, { 0.0f, 0.0f } },	// 0 base left
	{ {  0.5f,-0.5f, 0.5f, 1.0f },white, { 1.0f, 0.0f } },	// 1 top left
	{ {  0.5f, 0.5f, 0.5f, 1.0f },white, { 1.0f, 1.0f } },	// 2 top right
	{ { -0.5f, 0.5f, 0.5f, 1.0f },white, { 0.0f, 1.0f } },	// 3 base right

															// BACK FACE	- WHITE
	{ { -0.5f,-0.5f,-0.5f, 1.0f },white, { 0.0f, 0.0f } },	// 4 
	{ {  0.5f,-0.5f,-0.5f, 1.0f },white, { 1.0f, 0.0f } },	// 5
	{ {  0.5f, 0.5f,-0.5f, 1.0f },white, { 1.0f, 1.0f } },	// 6
	{ { -0.5f, 0.5f,-0.5f, 1.0f },white, { 0.0f, 1.0f } },	// 7

															// LEFT FACE	- WHITE
	{ { -0.5f,-0.5f,-0.5f, 1.0f },white, { 0.0f, 0.0f } },	// 8
	{ { -0.5f,-0.5f, 0.5f, 1.0f },white, { 0.0f, 1.0f } },	// 9
	{ { -0.5f, 0.5f, 0.5f, 1.0f },white, { 1.0f, 1.0f } },	// 10
	{ { -0.5f, 0.5f,-0.5f, 1.0f },white, { 1.0f, 0.0f } },	// 11

															// RIGHT FACE	- WHITE
	{ { 0.5f,-0.5f,-0.5f, 1.0f },white, { 0.0f, 0.0f } },	// 12
	{ { 0.5f,-0.5f, 0.5f, 1.0f },white, { 0.0f, 1.0f } },	// 13
	{ { 0.5f, 0.5f, 0.5f, 1.0f },white, { 1.0f, 1.0f } },	// 14
	{ { 0.5f, 0.5f,-0.5f, 1.0f },white, { 1.0f, 0.0f } },	// 15

															// TOP FACE		- WHITE
	{ {-0.5f, 0.5f,-0.5f, 1.0f },white, { 0.0f, 0.0f } },	// 16
	{ {-0.5f, 0.5f, 0.5f, 1.0f },white, { 0.0f, 1.0f } },	// 17
	{ { 0.5f, 0.5f, 0.5f, 1.0f },white, { 1.0f, 1.0f } },	// 18
	{ { 0.5f, 0.5f,-0.5f, 1.0f },white, { 1.0f, 0.0f } },	// 19

															// BOTTOM FACE	- WHITE
	{ {-0.5f,-0.5f,-0.5f, 1.0f },white, { 0.0f, 0.0f } },	// 20
	{ {-0.5f,-0.5f, 0.5f, 1.0f },white, { 0.0f, 1.0f } },	// 21
	{ { 0.5f,-0.5f, 0.5f, 1.0f },white, { 1.0f, 1.0f } },	// 22
	{ { 0.5f,-0.5f,-0.5f, 1.0f },white, { 1.0f, 0.0f } }	// 23
	};
	/*
	STEP 2:
	Describing Cube structure: using above Verts array, construct triangles for OpenGL to render
	Render Order: Each triangle to be described in clockwise order, to set correct facing direction per triangle
	OpenGL defaults to cull pixels that are 'facing away' from camera glCullMode(GL_BACK) is default setting.
	Culling can be changed to GL_FRONT or GL_FRONT_AND_BACK, or enabled/disabled via glEnable(GL_CULL_FACE) / glDisable(GL_CULL_FACE)
	*/
	unsigned char indices[] =
	{
		/*	TRIANGLE VERTEX BUILD
		(3)	X----X (2)
			|	/|
			|  / |
			| /	 |
		(0) X----X (1)
		*/
		0, 1, 2,	0, 2, 3,	// front face triangles
		6, 5, 4,	7, 6, 4,	// back face triangles
		8, 9,10,	8, 10,11,	// left face triangles
		14,13,12,	15,14,12,	// right face triangles
		16,17,18,	16,18,19,	// top face triangles
		22,21,20,	23,22,20	// bottom face triangles
	};

	/*
	STEP 3:
	Check: Sizes of Verts/Indices
	Drawing: glDrawElements method requires Indices number
	sizeof(verts) returns entire size in byes of array
	sizeof(Vertex) returns size of single vertes
	Calculate number of verts/indices by dividing
	*/
	// CHECK SIZE of DYNAMIC ARRAY
	m_cube.vertCount = sizeof(verts) / sizeof(Vertex);
	m_cube.indicesCount = sizeof(indices) / sizeof(unsigned char);

	// LIGHTING: LOOP THROUGH INDICIES (face = set of 3) AND THEREFORE FACES
	for (int i = 0; i < m_cube.indicesCount / 3; i++)
	{
		// FACES OF VERTEX
		int index0 = indices[i * 3 + 0];
		int index1 = indices[i * 3 + 1];
		int index2 = indices[i * 3 + 2];

		//SIDES OF VERTEX
		glm::vec4 side1 = glm::normalize(verts[index0].position - verts[index1].position);
		glm::vec4 side2 = glm::normalize(verts[index0].position - verts[index2].position);

		glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(side1), glm::vec3(side2)));
		// Add Normals to each face:
		verts[index0].normal += normal;
		verts[index1].normal += normal;
		verts[index2].normal += normal;
	}
	// Normals share multiple faces
	for (int i = 0; i < m_cube.vertCount; i++)
	{
		verts[i].normal = glm::normalize(verts[i].normal);
	}

	m_cube.indicesSizeType = GL_UNSIGNED_BYTE;

	/* STEP 4:
	Generate: VAO and Bind it */
	glGenVertexArrays(1, &m_cube.vao);
	glBindVertexArray(m_cube.vao);

	/* STEP 5:
	Create VBO and IBO
	Tell OpenGL Buffer Type and use
	VBO: Buffer in graphics memory to contain vertices
	IBO: Buffer in graphics memory to contain indices
	Fill buffers with generated data
	Sends Verts and Indices from CPU to GPU */
	// Generate
	glGenBuffers(1, &m_cube.vbo);
	glGenBuffers(1, &m_cube.ibo);
	// Bind
	glBindBuffer(GL_ARRAY_BUFFER, m_cube.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cube.ibo);
	// FILL BUFFERS with data size, pointer to first item in memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* STEP 6:
	Describe Vertices shape
	Example: Each vertex holds a position and colour
	Shape of vertex descibed to OpenGL - send to shader, and mapped to location */
	Vertex::SetupVertexAttribPointers();

	/* STEP 7:
	Unbind Buffers - pass in 0 */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void GraphicsAssessment::DestroyCube()
{
	glDeleteBuffers(1, &m_cube.vbo);
	glDeleteBuffers(1, &m_cube.ibo);
	glDeleteVertexArrays(1, &m_cube.vao);
}

void GraphicsAssessment::CreateGrid()
{
	/*
	STEP 1:
	Specify position, colour for each vert of a grid
	*/

	glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);	// Defined colour variable "White"
	//glm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);	// Defined colour variable "Red"
	//glm::vec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);	// Defined colour variable "Yellow"
	//glm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);	// Defined colour variable "Green"
	//glm::vec4 cyan(0.0f, 1.0f, 1.0f, 1.0f);	// Defined colour variable "Cyan"
	//glm::vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);	// Defined colour variable "Blue"
	//glm::vec4 magenta(1.0f, 0.0f, 1.0f, 1.0f);	// Defined colour variable "Magenta"

	std::vector<Vertex> verts;

	// Create Grid parameters
	const int xDiv = 20;	// m_heightmap->getWidth();	// Heightmap instead of 10;
	const int yDiv = 20;	//m_heightmap->getHeight();	// Heightmap pixels(r,g,b,a)
	float spacing = 1.0f;	// each pixel equals one unit, eg bitmap 128 x 128 units

	//TODO: algorithm to calculate vertices
	for (int y = 0; y < yDiv; y++)
	{
		for (int x = 0; x < xDiv; x++)
		{
			// HeightMap: Access Pixel index value - To sample pixel
			//unsigned int index = (y * xDiv + x);

			// Access Heightmap pixels (* pixel by bytes per pixel
			//const unsigned char *pixels = m_heightmap->getPixels();

			//float yPos = 0; pre heightmap
			// 3=red pixel bitmap image, 255=red, if blue [index*3+1]. green +2..
			//float yPos = (pixels[index * 3] / 255.0f) * 2.0f - 1.0f;

			// GRID plane going into the screen .: z & y inverted
			Vertex v;
			v.position.x = (x * spacing) - ((xDiv - 1) * spacing * 0.5f);
			v.position.z = (y * spacing) - ((yDiv - 1) * spacing * 0.5f);
			v.position.y = 0; // yPos; // heightmap
			v.position.w = 1.0f;
			v.colour = glm::vec4(1, 1, 1, 1); // Calculate Colour
			v.uv = glm::vec2(x / (float)(xDiv-1), y / (float)(yDiv-1)); // Calculate UV coords - as a percentage

			verts.push_back(v);
		}
	}
	
	/*
	STEP 2:
	Describing Grid structure: */
	std::vector<unsigned short> indices;

	// TODO: algorithm to calc faces
	// Grid of 5 verts going across, 2 down - Triangulate each square w.2 tri's.
	//	.	.	.	.	.
	//	.	.	.	.	.	=> Each vert has index value and position
	//	.	.	.	.	.
	// (0,0)
	for (int y = 0; y < yDiv - 1; y++)
	{
		for (int x = 0; x < xDiv - 1; x++)
		{
			// Loop each vert, bottom left i0 = x * xiv + x
			// Triangulate across to x2 Shift right -->  , then up, then across.
			unsigned short i0 = y * xDiv + x;
			unsigned short i1 = y * xDiv + (x + 1);
			unsigned short i2 = (y + 1) * xDiv + (x + 1);
			unsigned short i3 = (y + 1) * xDiv + x;

			indices.push_back(i0);	//
			indices.push_back(i2);	// => Each set of 3 index values equals a face
			indices.push_back(i1);	//

			indices.push_back(i0);
			indices.push_back(i3);
			indices.push_back(i2);
		}
	}

	// LOOP THROUGH INDICIES (face = set of 3)
	for (int i = 0; i < indices.size() / 3; i++)
	{
		// FACES OF VERTEX
		int index0 = indices[i * 3 + 0];
		int index1 = indices[i * 3 + 1];
		int index2 = indices[i * 3 + 2];

		//SIDES OF VERTEX
		glm::vec4 side1 = glm::normalize(verts[index0].position - verts[index1].position);
		glm::vec4 side2 = glm::normalize(verts[index0].position - verts[index2].position);

		glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(side1), glm::vec3(side2)));

		verts[index0].normal += normal;
		verts[index1].normal += normal;
		verts[index2].normal += normal;
	}

	for (int i = 0; i < verts.size() / 3; i++)
	{
		verts[i].normal = glm::normalize(verts[i].normal);
	}

	/*
	STEP 3:
	Check: Sizes of Verts/Indices
	Drawing: glDrawElements method requires Indices number
	sizeof(verts) returns entire size in byes of array
	sizeof(Vertex) returns size of single vertes
	Calculate number of verts/indices by dividing
	*/
	// CHECK SIZE of DYNAMIC ARRAY
	m_grid.vertCount = verts.size();
	m_grid.indicesCount = indices.size();

	m_grid.indicesSizeType = GL_UNSIGNED_SHORT;

	/* STEP 4:
	Generate: VAO and Bind it */
	glGenVertexArrays(1, &m_grid.vao);
	glBindVertexArray(m_grid.vao);

	/* STEP 5:
	Create VBO and IBO
	Tell OpenGL Buffer Type and use
	VBO: Buffer in graphics memory to contain vertices
	IBO: Buffer in graphics memory to contain indices
	Fill buffers with generated data
	Sends Verts and Indices from CPU to GPU */
	// Generate
	glGenBuffers(1, &m_grid.vbo);
	glGenBuffers(1, &m_grid.ibo);
	// Bind
	glBindBuffer(GL_ARRAY_BUFFER, m_grid.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_grid.ibo);
	// FILL BUFFERS with data size, pointer to first item in memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verts.size(), &verts[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), &indices[0], GL_STATIC_DRAW);

	/* STEP 6:
	Describe Vertices shape
	Example: Each vertex holds a position and colour
	Shape of vertex descibed to OpenGL - send to shader, and mapped to location */
	Vertex::SetupVertexAttribPointers();

	/* STEP 7:
	Unbind Buffers - pass in 0 */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void GraphicsAssessment::DestroyGrid()
{
	glDeleteBuffers(1, &m_grid.vbo);
	glDeleteBuffers(1, &m_grid.ibo);
	glDeleteVertexArrays(1, &m_grid.vao);
}

void GraphicsAssessment::LoadShader()
{
	// CREATE: Shader
	m_shaderProgram = new Shader("./shaders/Particle.vert", "./shaders/Particle.frag");
	// Shader::Load() moved to Shader.cpp
	m_projectionViewLoc = glGetUniformLocation(m_shaderProgram->GetProgramID(), "projectionView");
}

void GraphicsAssessment::UnloadShader()
{
	delete m_shaderProgram;
}

void GraphicsAssessment::soulSpear()
{
	Gizmos::draw(m_camera->getProjectionView());

	// Shader SoulSpear
	glUseProgram(m_shaderSoulSpear->GetProgramID());
	
	int loc = glGetUniformLocation(m_shaderSoulSpear->GetProgramID(), "projectionViewWorldMatrix");
	glUniformMatrix4fv(loc, 1, false, &m_camera->getProjectionView()[0][0]);
	
	loc = glGetUniformLocation(m_shaderSoulSpear->GetProgramID(), "modelMatrix");
	glm::mat4 modelMatrix(1);
	glUniformMatrix4fv(loc, 1, false, &modelMatrix[0][0]);

	// TEXTURES: SoulSpear
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_SoulSpearDiffuse->getHandle());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_SoulSpearNormal->getHandle());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_SoulSpearSpecular->getHandle());

	loc = glGetUniformLocation(m_shaderSoulSpear->GetProgramID(), "myTextureSampler");
	//assert(loc != -1);
	glUniform1i(loc, 0); // Change between, 0-2 to load different maps. (Diffuse, Normal & Specular.




	for (auto& renderData : m_SoulSpear)
	{
		if (m_camera->isBoundsInFulstrum(renderData->GetBounds()));
		{
			renderData->Render();
		}
	}
}

void Vertex::SetupVertexAttribPointers()
{
	/*	Vertex Attributes/Properties in array list
	Position (x,y,z,w)	- 4 bytes
	Colour (r,g,b,a)	- 4 bytes

	Length of entire vertex = stride (Position, colour) per vertex
	Offset allows jumping from one stride to next vertexes stride
	*/

	// POSITION: Enable vertex position element
	// Loading Shader: Position element is location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,							// Attribute 0 (Position)
		4,							// Size: How many floats make up position (x, y, z, w)
		GL_FLOAT,					// Type: Our x,y,z,w are float values
		GL_FALSE,					// Normalized - not used
		sizeof(Vertex),				// Stride: Size of an entire vertex
		(void*)0					// Offset - Bytes from beginning of the vertex
	);

	// COLOUR: Enable vertex colour element
	// Loading Shader: Colour element is location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,							// Attribute 1 (Colour) eg UV, attribute 2
		4,							// Size: How many floats make up colour (r,g,b,a) eg. if UV, 2 floats
		GL_FLOAT,					// Type: Our r,g,b,a are float values
		GL_FALSE,					// Normalized? - not used
		sizeof(Vertex),				// Stride: Size of entire vertex
		(void*)(sizeof(float) * 4)	// Offset - Bytes from beginning of the vertex. Position has 4 floats, .: 4 is how many we need to jump over
	);

	// UV: Enable texture element
	// Loading Shader: UV element is location 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,							// Attribute 1 (UV)
		2,							// Size: How many floats make up texture (u,v)
		GL_FLOAT,					// Type: Our u, v are float values
		GL_FALSE,					// Normalized? - not used
		sizeof(Vertex),				// Stride: Size of entire vertex
		(void*)(sizeof(float) * 8)	// Offset - Bytes from beginning of the vertex(position 4f, colour 4f, uv start = 8)
	);

	// NORMALS: 
	// Loading Shader: Normals element is location 3
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3,							// Attribute 1 (UV)
		3,							// Size: How many floats make up texture (x,y,z)
		GL_FLOAT,					// Type: Our u, v are float values
		GL_FALSE,					// Normalized? - not used
		sizeof(Vertex),				// Stride: Size of entire vertex
		(void*)(sizeof(float) * 10)	// Offset - Bytes from beginning of the vertex(position 4f, colour 4f, uv 2f, normals at 10)
	);
}
