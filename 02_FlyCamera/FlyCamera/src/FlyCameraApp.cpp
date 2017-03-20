#include "FlyCameraApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

FlyCameraApp::FlyCameraApp() {

}

FlyCameraApp::~FlyCameraApp() {

}

bool FlyCameraApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms: Sets Projection/Matrix
	m_camera = new Camera();
	m_camera->SetPosition(glm::vec3(10,10,10));
	m_camera->Lookat(glm::vec3(0,0,0));

	
	// Camera : Projection: Field of View, Aspect Ratio, 
	m_camera->SetProjection(
		glm::radians(45.0f),
		(float)getWindowWidth() / (float)getWindowHeight(),
		0.1f,
		1000.0f
	);

	return true;
}

void FlyCameraApp::shutdown() {

	delete m_camera;
	Gizmos::destroy();
}

void FlyCameraApp::update(float deltaTime) {

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	// ESC
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	// Inherited Camera update method
	m_camera->Update(deltaTime);
}

void FlyCameraApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// Projection : Camera Class
	glm::mat4 projection = m_camera->GetProjection();
	glm::mat4 view = m_camera->GetView();

	DrawGizmosGrid();
	Gizmos::draw(projection * view);
}

void FlyCameraApp::DrawGizmosGrid()
{
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

}
