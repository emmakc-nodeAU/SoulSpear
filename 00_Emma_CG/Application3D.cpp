#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Texture.h>
#include "RenderData.h"
#include "Camera.h"
#include "FlyCamera.h"
#include "Shader.h"
#include "GeometryHelper.h"
#include <gl_core_4_4.h>
#include <imgui.h>
#include <glfw3.h>
#include "ParticlesEmitter.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// GIZMOS Initialise Primitive Counts -------------------------
	Gizmos::create(10000, 10000, 10000, 10000);

	// CAMERA Virtual transforms ----------------------------------
	//m_viewMatrix = glm::lookAt(
	//	vec3(10), 
	//	vec3(0), 
	//	vec3(0, 1, 0));
	//m_projectionMatrix = glm::perspective(
	//	glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	// CAMERA - Upgrading to FlyCamera, from virtual camera -------
	m_camera = new FlyCamera(m_window, 5.0f);
	m_camera->setPerspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.0f);
	// CAMERA - VIEW TRANSFORM: Pos, Projection/Clip plane, Upright
	m_camera->lookAt
	(
		glm::vec3(10, 10, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	
	// TEXTURE
	//int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	//unsigned char* data = stbi_load("./textures/box_512x512.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	//glGenTextures(1, &m_text);
	//glBindTexture(GL_TEXTURE_2D, m_text);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//stbi_image_free(data);
	//const char* vsSource = "#version 410\n\n \
	//						layout(location = 0) in vec4 position; \
	//						layout(location = 1) in vec2 texCoord; \
	//						out vec2 vTexCoord;					   \
	//						uniform mat4 projectionViewWorldMatrix;   \
	//						void main()							   \
	//						{vTexCoord = texCoord; \
	//						gl_Position = projectionViewWorldMatrix * \
	//						position; \
	//						}";
	//
	//const char* fsSource = "#version 410\n\n \
	//						in vec2 vTexCoord; \
	//						out vec4 fragColor; \
	//						uniform sampler2D diffuse; \
	//						void main() \
	//						{ fragColor = texture(diffuse, vTexCoord); }\
	//						";
	//unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	//glCompileShader(vertexShader);
	//
	//unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	//glCompileShader(fragmentShader);
	//
	//m_program = glCreateProgram();
	//glAttachShader(m_program, vertexShader);
	//glAttachShader(m_program, fragmentShader);
	//glLinkProgram(m_program);
	//
	//int success = 0;
	//glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	//if (success == GL_FALSE)
	//{
	//	int infoLogLength = 0;
	//	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
	//	char* infoLog = new char[infoLogLength];
	//	glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
	//	printf("Error: Failed to link shader program!\n");
	//	printf("%s\n", infoLog); delete[] infoLog;
	//}
	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);
	//genTexture();

	// LIGHTING - Initialise Shader --------------------------------------------------
	m_lightingShader	= new Shader("./shaders/lighting.vert", "./shaders/lighting.frag");
	m_shaderProgram		= new Shader("./shaders/DiffuseLight.vert", "./shaders/DiffuseLight.frag");
	
	// GIZMOS - GRID: Initialise Shader ----------------------------------------------
	// Link to Shader
	m_gridShader		= new Shader("./shaders/grid.vert", "./shaders/grid.frag");
	// Link to Texture
	m_texture			= new aie::Texture("./textures/grass.png");

	// GIZMOS - CUBE: Initialise Shader ----------------------------------------------
	m_whiteTexture		= new aie::Texture("./textures/white.png");

	// MODELS - Initialise Model and Shader ------------------------------------------
	// BUNNY
<<<<<<< HEAD
	m_bunny				= GeometryHelper::loadObjFromFile("./models/stanford/Bunny.obj");
=======
	m_bunny			= GeometryHelper::loadObjFromFile("./models/stanford/Bunny.obj");
>>>>>>> 475a493c54406e2bd9d65a7a0a2e94f51751387b
	m_modelShader		= new Shader("./shaders/model.vert", "./shaders/model.frag");
	// SOULSPEAR
	m_SoulSpear			= GeometryHelper::loadObjFromFile("./models/soulspear/soulspear.obj");
	m_shaderSoulSpear	= new Shader("./shaders/soulspear.vert", "./shaders/soulspear.frag");

	// PARTICLES - Initialise
	m_emitter = new ParticlesEmitter();
	m_emitter->initalise(1000, 5000,
		0.1f, 1.0f,
		1, 5,
		1, 0.1f,
		glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));

	m_particleShaderProgramID = new Shader("./shaders/Particle.vert", "./shaders/Particle.frag");

	return true;
}

void Application3D::shutdown() {
	
	// DELETE: CAMERA--------------

	// DELETE: MODELS--------------
	
	// DELETE: TEXTURES------------
	delete m_whiteTexture;
	delete m_texture;
	
	// DELETE: SHADERS-------------
	delete m_shaderSoulSpear;
	delete m_modelShader;
	delete m_gridShader;
	delete m_lightingShader;
	delete m_shaderProgram;
	// DELETE: GIZMOS--------------
	delete m_gridRenderData;
	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// GIZMOS FRAME CLEANUP		-----------------------------------
	Gizmos::clear();

	// CAMERA - Rotation		-----------------------------------
	float time = getTime();
	m_viewMatrix = glm::lookAt
	(
		vec3(
		glm::sin(time) * 10, 
		10, 
		glm::cos(time) * 10), 
		vec3(0), 
		vec3(0, 1, 0)
	);
	m_camera->Update(deltaTime);

	// GIZMOS GRID USING LINES -----------------------------------
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
	// GIZMOS: LINE TRANSFORM, SHOWS AN 'AXIS' --------------------
	Gizmos::addTransform(mat4(1));

	// ESCAPE TO QUIT ---------------------------------------------
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	// LIGHTING			-------------------------------------------
	m_lightPosition.x = glm::cos(time) * 5;
	m_lightPosition.z = glm::sin(time) * 5;
	Gizmos::addSphere(
		m_lightPosition,				// Position
		0.1f,							// RADIUS
		16, 16,							// SPHERE SIZE
		glm::vec4(m_lightColour, 1.0f));// COLOUR

	// IMGUI Controls	-------------------------------------------
	ImGui::SliderFloat("AmbientStrength", &m_ambientStrength, 0.0f, 1.0f);
	ImGui::SliderFloat3("LightColour", &m_lightColour[0], 0.0f, 1.0f);

	// PARTICLES		-------------------------------------------
	m_emitter->update(deltaTime, m_camera->getTransform());
}

void Application3D::draw() {

	// SCREEN WIPE ------------------------------------------------
	clearScreen();

	// CAMERA - Update perspective based on screen size -----------
	m_projectionMatrix = glm::perspective(
		glm::pi<float>() * 0.25f, 
		getWindowWidth() / 
		(float)getWindowHeight(), 
		0.1f, 1000.0f);
	// CAMERA - TRANSFORM PROJECTION (Perspective) ----------------
	Gizmos::draw(m_camera->getProjectionView());
	
	// TEXTURES Test
	//glUseProgram(m_program);
	//
	//int loc = glGetUniformLocation(m_program, "projectionViewWorldMatrix");
	//glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_camera->getProjectionView()[0][0]));
	//
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_text);
	//
	//loc = glGetUniformLocation(m_program, "diffuse");
	//glUniform1i(loc, 0);
	//
	//glBindVertexArray(m_vao);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// GIZMOS GRID - Shader ---------------------------------------
	glm::mat4 projView = m_projectionMatrix * m_viewMatrix;
	glUseProgram(m_gridShader->GetProgramID());
	int loc = glGetUniformLocation(m_gridShader->GetProgramID(), "projectionViewMatrix");
	assert(loc != -1);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(projView));
	
	glm::mat4 modelMatrix(1);
	loc = glGetUniformLocation(m_gridShader->GetProgramID(), "modelMatrix");
	assert(loc != -1);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(modelMatrix));

	// MODEL Bunny - Shader ---------------------------------------
	glUseProgram(m_shaderSoulSpear->GetProgramID());
	loc = glGetUniformLocation(m_shaderSoulSpear->GetProgramID(), "projectionViewWorldMatrix");
	assert(loc != -1);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(m_camera->getProjectionView()));
	loc = glGetUniformLocation(m_shaderSoulSpear->GetProgramID(), "modelMatrix");
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(m_camera->getProjectionView()));

	// MODEL Bunny - Render ---------------------------------------
	for (auto& mesh : m_bunny)
	{
		mesh->Bind();
		glDrawArrays(GL_TRIANGLES, 0, mesh->getNumberOfIndicies());
	}

	// MODEL Soulspear - Shader -----------------------------------
	glUseProgram(m_shaderSoulSpear->GetProgramID());
	loc = glGetUniformLocation(m_shaderSoulSpear->GetProgramID(), "projectionViewWorldMatrix");
	assert(loc != -1);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(m_camera->getProjectionView()));
	loc = glGetUniformLocation(m_shaderSoulSpear->GetProgramID(), "modelMatrix");
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(m_camera->getProjectionView()));

	for (auto& mesh : m_SoulSpear)
	{
		mesh->Bind();
		glDrawArrays(GL_TRIANGLES, 0, mesh->getNumberOfIndicies());
	}
	
	// LIGHTING - Shader (Diffuse) --------------------------------
	glUseProgram(m_shaderProgram->GetProgramID());
	m_projectionViewLoc = glGetUniformLocation(m_shaderProgram->GetProgramID(), "projectionView");
	glUniformMatrix4fv(m_projectionViewLoc, 1, false, glm::value_ptr(m_camera->getProjectionView()));

	glUniform1fv(glGetUniformLocation(m_shaderProgram->GetProgramID(), "lightAmbientStrength"), 1, &m_ambientStrength);
	glUniform3fv(glGetUniformLocation(m_shaderProgram->GetProgramID(), "lightColour"), 1, &m_lightColour[0]); // Light colour
	glUniform3fv(glGetUniformLocation(m_shaderProgram->GetProgramID(), "lightPos"), 1, &m_lightPosition[0]); // Light Position

	// Step 6: Deactivate Shader program
	glUseProgram(0);
	Gizmos::draw(m_camera->getProjectionView());
}

void Application3D::RenderMesh(GLMesh *mesh, glm::vec3 &position, glm::vec3 &scale, glm::vec3 materialColour, aie::Texture *diffuseTexture)
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
//void Application3D::genTexture()
//{
//	float vertexData[] =
//	{
//		-5, 0, 5, 1, 0 ,1,
//		5, 0, 5, 1, 1, 1,
//		5, 0, -5, 1, 1, 0,
//		-5, 0, -5, 1, 0, 0,
//	};
//
//	unsigned int indexData[] =
//	{
//		0, 1, 2,
//		0, 2, 3,
//	};
//
//	glGenVertexArrays(1, &m_vao);
//	glBindVertexArray(m_vao);
//
//	glGenBuffers(1, &m_vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4,
//		vertexData, GL_STATIC_DRAW);
//
//	glGenBuffers(1, &m_ibo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,
//		indexData, GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
//		sizeof(float) * 6, 0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
//		sizeof(float) * 6, ((char*)0) + 16);
//
//	glBindVertexArray(0);
//
//}