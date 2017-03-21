/*
-------------------------------------------------------------------------
INSTRUCTIONS:
-------------------------------------------------------------------------
STEP 1: Generate Geometry
		Create buffers on GPU and populate with data.

STEP 2: Load a shader program
		See LoadShader method

STEP 3:	Each frame - Render Geometry (using shader program)
		See DrawGeometry method

STEP 4:	Unload Shader and Geometry
-------------------------------------------------------------------------
*/

#include "RenderingGeometryApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Texture.h>

#include "Camera.h"

#include <gl_core_4_4.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

RenderingGeometryApp::RenderingGeometryApp() {

}

RenderingGeometryApp::~RenderingGeometryApp() {

}

bool RenderingGeometryApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// CAMERA
	m_camera = new Camera();
	m_camera->SetPosition(glm::vec3(10,10,10));
	m_camera->Lookat(glm::vec3(0, 0, 0));
	m_camera->SetProjection(glm::radians(45.0f), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	// TEXTURE
	m_texture  = new aie::Texture("./textures/box_512x512.jpg");
	m_texture1 = new aie::Texture("./textures/grass.png");
	//m_heightmap = new aie::Texture("./textures/moss2-heightmap.bmp");

	// SHADER
	LoadShader();

	// GEOMETRY
	CreateCube();
	CreateGrid();

	return true;
}

void RenderingGeometryApp::shutdown() {
	
	//delete m_heightmap;
	delete m_texture1;
	delete m_texture;
	DestroyGrid();
	DestroyCube();
	UnloadShader();

	delete m_camera;
	Gizmos::destroy();
}

void RenderingGeometryApp::update(float deltaTime) {

	m_camera->Update(deltaTime);

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
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

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void RenderingGeometryApp::draw() {

	// Camera
	glm::mat4 projection = m_camera->GetProjection();
	glm::mat4 view = m_camera->GetView();

	// wipe the screen to the background colour
	clearScreen();

	// Step 1: Before rendering geometry, tell OpenGl to use Shader Program
	glUseProgram(m_shader);

	// Step 2: Calculate projection view matrix, pass into shader program
	glm::mat4 projectionView = projection * view;
	glUniformMatrix4fv(m_projectionViewLoc, 1, false, &projectionView[0][0]);
	
	// Texture: CUBE: GPU texture slot zero(0)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->getHandle());
	glUniform1i(glGetUniformLocation(m_shader, "texture"), 0);

	// Step 3: CUBE: Render and Bind VAO
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(m_cube.vao);

	// Step 4:	CUBE: Draw Elements: GL_TRIANGLES
	//			Tell OpenGL number and size of Indices (each a 1 byte unsigned char)
	glDrawElements(GL_TRIANGLES, m_cube.indicesCount, GL_UNSIGNED_BYTE, 0);

	// Texture: GRID: GPU texture slot zero(0)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture1->getHandle());
	glUniform1i(glGetUniformLocation(m_shader, "texture"), 0);

	// Step 3: GRID: Render and Bind VAO
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// GL_LINE Wireframe mode, GL_FILL Solid
	glBindVertexArray(m_grid.vao);

	// Step 4:	GRID: Draw Elements: GL_TRIANGLES
	glDrawElements(GL_TRIANGLES, m_grid.indicesCount, GL_UNSIGNED_SHORT, 0);


	// Step 5: Unbind VAO, cleanup OpenGL
	glBindVertexArray(0);

	// Step 6: Deactivate Shader program
	glUseProgram(0);
	
	Gizmos::draw(projection * view);
}

void RenderingGeometryApp::CreateCube()
{
	/*
	STEP 1: 
	Specify position, colour for each vert of a cube
	Example: Each face does not shar ea vert, 4 verts per face of cube
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

void RenderingGeometryApp::DestroyCube()
{
	glDeleteBuffers(1, &m_cube.vbo);
	glDeleteBuffers(1, &m_cube.ibo);
	glDeleteVertexArrays(1, &m_cube.vao);
}

void RenderingGeometryApp::CreateGrid()
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
	const int xDiv = 10; // m_heightmap->getWidth();	// Heightmap instead of 10;
	const int yDiv = 10; // m_heightmap->getHeight();	// Heightmap pixels(r,g,b,a)
	float spacing = 0.1f;

	//TODO: algorithm to calculate vertices
	for (int y = 0; y < yDiv; y++)
	{
		for (int x = 0; x < xDiv; x++)
		{
			// Access Pixel index value - used to sample pixel
			unsigned int index = (y * xDiv + x);

			// Access Heightmap pixels
			//const unsigned char *pixels = m_heightmap->getPixels();
			//float yPos = (pixels[index * 3] / 255.0f) * 2.0f - 1.0f;	// 3=red pixel bitmap image, 255=red, if blue [index*3+1]. green +2..

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

			indices.push_back(i0);
			indices.push_back(i2);
			indices.push_back(i1);

			indices.push_back(i0);
			indices.push_back(i3);
			indices.push_back(i2);
		}
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

void RenderingGeometryApp::DestroyGrid()
{
	glDeleteBuffers(1, &m_grid.vbo);
	glDeleteBuffers(1, &m_grid.ibo);
	glDeleteVertexArrays(1, &m_grid.vao);
}

void RenderingGeometryApp::LoadShader()
{
	// SHADER: Vertex
	/*	Position:	attribute 0
		Colour:		attribute 1 */
	static const char* vertex_shader =
		"#version 410\n \
						in vec4 vPosition;\n \
						in vec4 vColour;\n \
						in vec2 vUv; \n \
						out vec4 fColour;\n \
						out vec2 fuv; \n \
						uniform mat4 projectionView;\n \
						void main ()\n \
						{\n \
							fuv = vUv; \n\
							fColour = vColour; \n\
							gl_Position = projectionView * vPosition; \n\ }";

	// SHADER: Fragment
	static const char* fragment_shader =
		"#version 410\n \
						in vec4 fColour;\n \
						in vec2 fuv; \n \
						out vec4 frag_colour;\n \
						uniform sampler2D texture; \n\
						void main ()\n \
						{\n \
							frag_colour = texture2D(texture, fuv) * fColour;\n \
						}" ;
	int success = GL_FALSE;	// Error checking 

	// LOAD GEOMETRY
	/* STEP 1:
	Create vertex shader, link to source code, compile it */
	// Create: Vertex Shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	/*STEP 2:
	Create fragment shader, link to source code, compile it */
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	/* STEP 3:
	Create Shader Program */
	m_shader = glCreateProgram();

	/* STEP 4:
	Attach vs & fs to shader program. */
	glAttachShader(m_shader, vs);
	glAttachShader(m_shader, fs);

	/* STEP 5:
	Describe attribute location of shader elements
	NOTE: Method: SetupVertexAttribPointers
	Used to describe verts. */
	glBindAttribLocation(m_shader, 0, "vPosition");
	glBindAttribLocation(m_shader, 1, "vColour");
	glBindAttribLocation(m_shader, 2, "vUv");
	// Link: Vert and Frag
	glLinkProgram(m_shader);
	m_projectionViewLoc = glGetUniformLocation(m_shader, "projectionView");

	// ERROR HELPER
	glGetProgramiv(m_shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infolog = new char[infoLogLength];

		glGetProgramInfoLog(m_shader, infoLogLength, 0, infolog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infolog);
		delete[] infolog;
	}

	/* STEP 6:
	Destroy vertex and fragment shaders
	They are now combined into shaderProgram*/
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void RenderingGeometryApp::UnloadShader()
{
	glDeleteProgram(m_shader);
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
}
