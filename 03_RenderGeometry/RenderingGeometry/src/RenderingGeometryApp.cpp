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

	// create simple camera transforms
	m_camera = new Camera();
	m_camera->SetPosition(glm::vec3(10,10,10));
	m_camera->Lookat(glm::vec3(0, 0, 0));
	m_camera->SetProjection(glm::radians(45.0f), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	return true;
}

void RenderingGeometryApp::shutdown() {

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

	glm::mat4 projection = m_camera->GetProjection();
	glm::mat4 view = m_camera->GetView();

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	
	Gizmos::draw(projection * view);
}

void RenderingGeometryApp::CreateGeometry()
{
	/*
	STEP 1: 
	Specify position, colour for each vert of a cube
	Example: Each face does not shar ea vert, 4 verts per face of cube
	*/
	Vertex verts[] =
	{
															// {POSITION} , {COLOUR}
															// FRONT FACE	- RED
	{ {-0.5f,-0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 0.5f } },	// 0
	{ { 0.5f,-0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 0.5f } },	// 1
	{ { 0.5f, 0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 0.5f } },	// 2
	{ {-0.5f,-0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 0.5f } },	// 3

															// BACK FACE	- YELLOW
	{ { -0.5f,-0.5f,-0.5f, 1.0f },{ 1.0f, 1.0f, 0.0f, 0.5f } },	// 4
	{ {  0.5f,-0.5f,-0.5f, 1.0f },{ 1.0f, 1.0f, 0.0f, 0.5f } },	// 5
	{ {  0.5f, 0.5f,-0.5f, 1.0f },{ 1.0f, 1.0f, 0.0f, 0.5f } },	// 6
	{ { -0.5f, 0.5f,-0.5f, 1.0f },{ 1.0f, 1.0f, 0.0f, 0.5f } },	// 7

															// LEFT FACE	- GREEN
	{ { -0.5f,-0.5f,-0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f } },	// 8
	{ { -0.5f,-0.5f, 0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f } },	// 9
	{ { -0.5f, 0.5f, 0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f } },	// 10
	{ { -0.5f, 0.5f,-0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f } },	// 11

															// RIGHT FACE	- CYAN
	{ { 0.5f,-0.5f,-0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f } },	// 12
	{ { 0.5f,-0.5f, 0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f } },	// 13
	{ { 0.5f, 0.5f, 0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f } },	// 14
	{ { 0.5f, 0.5f,-0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f } },	// 15

															// TOP FACE		- BLUE
	{ {-0.5f, 0.5f,-0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 0.5f } },	// 12
	{ {-0.5f, 0.5f, 0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 0.5f } },	// 13
	{ { 0.5f, 0.5f, 0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 0.5f } },	// 14
	{ { 0.5f, 0.5f,-0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 0.5f } },	// 15

															// BOTTOM FACE	- MAGENTA
	{ {-0.5f,-0.5f,-0.5f, 1.0f },{ 1.0f, 0.0f, 1.0f, 0.5f } },	// 12
	{ {-0.5f,-0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 1.0f, 0.5f } },	// 13
	{ { 0.5f,-0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 1.0f, 0.5f } },	// 14
	{ { 0.5f,-0.5f,-0.5f, 1.0f },{ 1.0f, 0.0f, 1.0f, 0.5f } },	// 15
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
	m_vertCount = sizeof(verts) / sizeof(Vertex);
	m_indicesCount = sizeof(indices) / sizeof(unsigned char);

	/*
	STEP 4:
	Generate: VAO and Bind it
	Group: VBO and IBO will be grouped with VAO
	*/
	// Generate Buffers:
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	/*
	STEP 5:
	Create VBO and IBO
	Tell OpenGL Buffer Type and use
	VBO: Buffer in graphics memory to contain vertices
	IBO: Buffer in graphics memory to contain indices
	Fill buffers with generated data
	Sends Verts and Indices from CPU to GPU
	*/
	// BIND BUFFERS
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	// FILL BUFFERS with data size, pointer to first item in memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*
	STEP 6:
	Describe Vertices shape
	Example: Each vertex holds a position and colour
	Shape of vertex descibed to OpenGL - send to shader, and mapped to location
	*/
	Vertex::SetupVertexAttribPointers();

	/*
	STEP 7:
	Unbind Buffers - pass in 0
	*/
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void RenderingGeometryApp::DestroyGeometry()
{
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
		1,							// Attribute 1 (Colour)
		4,							// Size: How many floats make up colour (r,g,b,a)
		GL_FLOAT,					// Type: Our r,g,b,a are float values
		GL_FALSE,					// Normalized? - not used
		sizeof(Vertex),				// Stride: Size of entire vertex
		(void*)(sizeof(float) * 4)	// Offset - Bytes from beginning of the vertex. Position has 4 floats, .: 4 is how many we need to jump over.
	);



}
