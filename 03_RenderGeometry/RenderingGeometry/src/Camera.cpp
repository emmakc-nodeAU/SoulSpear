#include "Camera.h"
#include <Input.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	// MOVE FORWARD
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		// Camera move forward 10 units per second
		m_cameraPosition += m_cameraFront * m_cameraMoveSpeed * deltaTime;
	}
	// MOVE BACK
	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		// Camera move backward 10 units per second
		m_cameraPosition -= m_cameraFront * m_cameraMoveSpeed * deltaTime;
	}
	// STRAIFE LEFT
	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		// Move perpendicular to facing direction, not just on x/y/z
		glm::vec3 dir = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
		m_cameraPosition -= dir * m_cameraMoveSpeed * deltaTime;
	}
	// STRAIFE RIGHT
	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		// Move perpendicular to facing direction, not just on x/y/z
		glm::vec3 dir = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
		m_cameraPosition += dir * m_cameraMoveSpeed * deltaTime;
	}

	// MOUSE Click and DRAG
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		// Mouse - Current position X & Y
		int mouseXPos, mouseYPos;
		input->getMouseXY(&mouseXPos, &mouseYPos);

		// Affects Camera rotation - Yaw/Pitch/Roll
		float deltaMouseX = mouseXPos - m_lastMouseXPos;
		float deltaMouseY = mouseYPos - m_lastMouseYPos;

		// If Dragging Left/Right
		m_cameraYaw += deltaMouseX;
		// If Dragging Up/Down
		m_cameraPitch += deltaMouseY;

		// Clamp pitch rotation
		if (m_cameraPitch > 85.0f) m_cameraPitch = 85.0f;
		if (m_cameraPitch < -85.0f) m_cameraPitch = -85.0f;

		CalculateFront();
	}

	// Camera - View Matrix
	m_viewMatrix = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);

	// Mouse - Recent position before next frame
	input->getMouseXY(&m_lastMouseXPos, &m_lastMouseYPos);
}


void Camera::CalculateFront()
{
	// Camera - Front Direction: Convert Yaw/Pitch/Roll into a facing front direction:
	glm::vec3 front;
	front.x = glm::cos(glm::radians(m_cameraYaw)) * glm::cos(glm::radians(m_cameraPitch));
	front.y = glm::sin(glm::radians(m_cameraPitch)) * glm::cos(glm::radians(m_cameraPitch));
	front.z = glm::tan(glm::radians(m_cameraRoll)) * glm::cos(glm::radians(m_cameraPitch));
	// Assign to front variable
	m_cameraFront = glm::normalize(front);
}

// SET
void Camera::SetProjection(float fov, float aspect, float near, float far)
{
	m_projectionMatrix = glm::perspective(fov, aspect, near, far);
}

void Camera::SetPosition(const glm::vec3& pos)
{
	m_cameraPosition = pos;
}

void Camera::SetYawPitchRoll(float yaw, float pitch, float roll)
{
	m_cameraYaw = yaw;
	m_cameraPitch = pitch;
	m_cameraRoll = roll;
	CalculateFront();

}

// GET
glm::vec3& Camera::GetPosition()
{
	return m_cameraPosition;
}

float Camera::GetYaw()
{
	return m_cameraYaw;
}

float Camera::GetPitch()
{
	return m_cameraPitch;
}

float Camera::GetRoll()
{
	return m_cameraRoll;
}

glm::mat4& Camera::GetView()
{
	return m_viewMatrix;
}

glm::mat4& Camera::GetProjection()
{
	return m_projectionMatrix;
}

void Camera::Lookat(glm::vec3 target)
{
	// Target position - calculate camera direction
	glm::vec3 dir = glm::normalize(target - m_cameraPosition);
	m_cameraPitch = glm::degrees(glm::asin(dir.y));
	m_cameraYaw = glm::degrees(atan2(dir.y, dir.x));

	CalculateFront();
}
