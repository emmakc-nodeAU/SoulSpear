#include "FlyCamera.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>



FlyCamera::FlyCamera(GLFWwindow * window, float speed)
	: m_speed(speed)
	, m_rotate(glm::pi<float>() * 0.05f)
	, m_window(window)
{

}

void FlyCamera::Update(float deltaTime)
{
	glm::mat4 transform = getTransform();
	glm::vec3 vRight	= glm::vec3(transform[0]);
	glm::vec3 vUp		= glm::vec3(transform[1]);
	glm::vec3 vForward	= glm::vec3(transform[2]);

	glm::vec3 moveDirection;

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		moveDirection -= (vForward * m_speed);
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		moveDirection -= (vRight * m_speed);
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		moveDirection += (vForward * m_speed);
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		moveDirection += (vRight * m_speed);
	}

	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		moveDirection += (vUp * m_speed);
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		moveDirection -= (vUp * m_speed);
	}

	if (glm::length(moveDirection) > 0.0f)
	{
		transform[3] += glm::vec4(moveDirection * deltaTime, 1);
		transform[3].w = 1;
		setTransform(transform);
	}
	
	if (glfwGetKey(m_window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		mouseInput(deltaTime);
	}
}

void FlyCamera::mouseInput(double deltaTime)
{
	glm::vec3 position = glm::vec3(0, 0, 5);
	float horizontalAngle = glm::pi<float>() * 0.25f;
	float verticleAngle = 0.0f;
	float mouseSpeed = 0.005f;

	double xPos, yPos;

	int windowWidth, windowHeight;
	glfwGetCursorPos(m_window, &xPos, &yPos);
	// Locks Mouse position to middle
	glfwSetCursorPos(m_window, windowWidth / 2, windowHeight / 2);

	float xOffset = windowWidth / 2 - xPos;
	float yOffset = windowHeight / 2 - yPos;

	horizontalAngle += mouseSpeed * deltaTime * xOffset;
	verticleAngle += mouseSpeed * deltaTime * yOffset;



	if (xOffset > 0)
		int i = 0;

	glm::vec3 direction(
		cos(verticleAngle) * sin(horizontalAngle),	// X
		sin(verticleAngle),							// Y
		cos(verticleAngle) * cos(horizontalAngle));	// Z

	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - glm::pi<float>() * 0.25f / 2.0f),
		0,
		cos(horizontalAngle - glm::pi<float>() * 0.25f / 2.0f)
	);

	glm::vec3 up = glm::cross(right, direction);

	getDirectionRotation(deltaTime, xOffset, yOffset);
}

void FlyCamera::getDirectionRotation(double deltaTime, double xPos, double yPos)
{
	if (yPos != 0.0f)
	{
		glm::mat4 rot = glm::rotate((float)(m_rotate * deltaTime * yPos), glm::vec3(1, 0, 0));
		setTransform(getTransform()*rot);
	}

	if (xPos != 0.0f)
	{
		glm::mat4 rot = glm::rotate((float)(m_rotate * deltaTime * yPos), glm::vec3(0, 1, 0));
		setTransform(getTransform()*rot);
	}

	glm::mat4 oldTrans = getTransform();
	glm::mat4 trans;
	glm::vec3 worldUp = glm::vec3(0, 1, 0);

	// LEFT & RIGHT
	glm::vec3 oldForward = glm::vec3(oldTrans[2].x, oldTrans[2].y, oldTrans[2].z);
	trans[0] = glm::normalize(glm::vec4(glm::cross(worldUp, oldForward), 0));
	// UP
	glm::vec3 newRight = glm::vec3(trans[0].x, trans[0].y, trans[0].z);
	trans[1] = glm::normalize(glm::vec4(glm::cross(oldForward, newRight), 0));

	// FORWARD
	trans[2] = glm::normalize(oldTrans[2]);

	// POSITION
	trans[2] = oldTrans[3];
	setTransform(trans);
}
