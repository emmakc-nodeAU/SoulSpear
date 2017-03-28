#include "FlyCamera.h"
#include <gl_core_4_4.h>
#include <glfw3.h>

FlyCamera::FlyCamera(GLFWwindow* window, float speed)
	: m_speed(speed)
	, m_rotate(glm::pi<float>() * 0.05f)
	, m_window(window)
{
}

void FlyCamera::Update(float deltaTime)
{
	glm::mat4 transform = getTransform();
	glm::vec4 vRight = transform[0];
	glm::vec4 vUp = transform[1];
	glm::vec4 vForward = transform[2];

	glm::vec2 rotAmount(0);
	glm::mat4 yRot(1);
	glm::mat4 xRot(1);
	glm::vec4 moveDirection;

	// W A S D Keybindings -------------------------------------
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		moveDirection -= (vForward * m_speed);
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		moveDirection += (vForward * m_speed);
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		moveDirection -= (vRight * m_speed);
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		moveDirection += (vRight * m_speed);
	}
	// E - UP ----------------------------------------------
	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		moveDirection += (vUp * m_speed);
	}
	// Q - DOWN ------------------------------------------
	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		moveDirection -= (vUp * m_speed);
	}
	// ARROW LEFT ROTATE
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		rotAmount.y		+= glm::pi<float>() * 0.6f * deltaTime;
		glm::vec4 vUp	= m_viewTransform * glm::vec4(0, 1, 0, 0);
		yRot			= glm::rotate(rotAmount.y, glm::vec3(vUp.x, vUp.y, vUp.z));
	}
	// ARROW RIGHT ROTATE
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		rotAmount.y		-= glm::pi<float>() * 0.6f * deltaTime;
		glm::vec4 vUp	= m_viewTransform * glm::vec4(0, 1, 0, 0);
		yRot			= glm::rotate(rotAmount.y, glm::vec3(vUp.x, vUp.y, vUp.z));
	}

	// UPDATE ARROWS for TRANSFORM
	//m_worldTransform = m_worldTransform * xRot;
	m_worldTransform = m_worldTransform * yRot;

	if (glm::length(moveDirection) > 0.0f)
	{
		transform[3] += moveDirection * deltaTime;
		setTransform(transform);
	}
	m_viewTransform = glm::inverse(m_worldTransform);

//	mouseInput(deltaTime);
}

//void FlyCamera::mouseInput(double dt)
//{
//	glm::vec3 position		= glm::vec3(0, 0, 5); 
//	float horizontalAngle	= glm::pi<float>() * 0.25f;
//	float verticleAngle		= 0.0f;
//	float mouseSpeed		= 0.005f;
//
//	// MOUSE PAN CAMERA VIEW (MOUSE IN SCREEN SPACE), (CAMERA IN WORLD SPACE)
//	double xPos, yPos;		// ScreenSpace 2D coords x,y
//	// GLFW Context CLIP SPACE
//	int windowWidth, windowHeight;
//	glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
//	glfwGetCursorPos(m_window, &xPos, &yPos);
//	glfwSetCursorPos(m_window, windowWidth / 2, windowHeight / 2);
//	float xOffset = windowWidth / 2 - xPos;
//	float yOffset = windowHeight / 2 - yPos;
//	horizontalAngle += mouseSpeed * dt * xOffset;
//	verticleAngle += mouseSpeed * dt * yOffset;
//
//	// MOUSE - TRANSFORM CLIP TO SCREEN SPACE COORDS
//	if (xOffset > 0)
//		int i = 0;
//	glm::vec3 direction(
//		cos(verticleAngle) * sin(horizontalAngle),
//		sin(verticleAngle),
//		cos(verticleAngle) * cos(horizontalAngle)
//	);
//
//	glm::vec3 right = glm::vec3(
//		sin(horizontalAngle - glm::pi<float>() * 1.f / 2.0f),
//		0,
//		cos(horizontalAngle - glm::pi<float>() * 1.f / 2.0f)
//	);
//
//	glm::vec3 up = glm::cross(right, direction);
//	getDirectionRotation(dt, xOffset, yOffset);
//
//}
//
//void FlyCamera::getDirectionRotation(double dt, double xPos, double yPos)
//{
//	// CAMERA ROTATION around X, Y
//	if (yPos != 0.0f)
//	{
//		glm::mat4 rot = glm::rotate(
//			(float)(m_rotate * dt * yPos), 
//			glm::vec3(1, 0, 0));
//		setTransform(getTransform()*rot);
//	}
//
//	if (xPos != 0.0f)
//	{
//		glm::mat4 rot = glm::rotate(
//			(float)(m_rotate * dt * xPos), 
//			glm::vec3(0, 1, 0));
//		setTransform(getTransform()*rot);
//	}
//
//	// CAMERA to MOUSE TRANSFORM
//	/*	CAMERA			MOUSE
//		[0][1][2*][3]	[0*][1*][2][3]
//		[0][1][2*][3]	[0*][1*][2][3]
//		[0][1][2*][3]	[0*][1*][2][3]
//		[0][1][2 ][3]	[0 ][1 ][2][3]
//
//		cameraTransform = [2].xyz
//		mouseTransform	= [0].xyz
//
//	*/
//	glm::mat4 cameraTransform = getTransform();
//	glm::mat4 mouseTransform;					 
//	glm::vec3 worldUp = glm::vec3(0, 1, 0);
//
//	// MOUSE PAN RIGHT
//	glm::vec3 cameraForward = glm::vec3(
//		cameraTransform[2].x,
//		cameraTransform[2].y,
//		cameraTransform[2].z);
//
//	// [0] MOUSE - Right ? PAN RIGHT assigned to CAMERA TRANSFORM
//	mouseTransform[0] = glm::normalize(
//		glm::vec4(glm::cross(worldUp, cameraForward), 0));
//	
//	// [1] MOUSE - Transform (Up?)
//	glm::vec3 mouseRight = glm::vec3(
//		mouseTransform[0].x,
//		mouseTransform[0].y,
//		mouseTransform[0].z);
//	mouseTransform[1] = glm::normalize(
//		glm::vec4(glm::cross(cameraForward, mouseRight), 0));
//
//	// [2] MOUSE - Transform (Forward?)
//	mouseTransform[2] = glm::normalize(cameraTransform[2]);
//
//	// [3] MOUSE - Transform (Position?)
//	mouseTransform[3] = cameraTransform[3];
//	setTransform(mouseTransform);
//}

