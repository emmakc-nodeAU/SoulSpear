#include "Camera.h"
#include <Input.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "SphereBoundingVolume.h"

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

void Camera::UpdateFrustrumPlanes()
{
	// UpdateFrustrumPlanes - inside anywhere that updates matrix projection/view
	glm::mat4& projView = GetProjectionView();
	// Right Side
	m_frustrumPlanes[0] = glm::vec4(
	projView[0][3] - projView[0][[0]
	projView[1][3] - projView[1][[0]
	projView[2][3] - projView[2][[0]
	projView[3][3] - projView[3][[0])

	// Left Side
	m_frustrumPlanes[1] = glm::vec4(
	projView[0][3] + projView[0][[0]
	projView[1][3] + projView[1][[0]
	projView[2][3] + projView[2][[0]
	projView[3][3] + projView[3][[0])

	// Top Side
	m_frustrumPlanes[2] = glm::vec4(
	projView[0][3] - projView[0][[1]
	projView[1][3] - projView[1][[1]
	projView[2][3] - projView[2][[1]
	projView[3][3] - projView[3][[1])
	
	// Bottom Side
	m_frustrumPlanes[3] = glm::vec4(
	projView[0][3] + projView[0][[1]
	projView[1][3] + projView[1][[1]
	projView[2][3] + projView[2][[1]
	projView[3][3] + projView[3][[1])
	
	// Far Side
	m_frustrumPlanes[4] = glm::vec4(
	projView[0][3] - projView[0][[2]
	projView[1][3] - projView[1][[2]
	projView[2][3] - projView[2][[2]
	projView[3][3] - projView[3][[2])
	
	// Near Side
	m_frustrumPlanes[5] = glm::vec4(
	projView[0][3] + projView[0][[2]
	projView[1][3] + projView[1][[2]
	projView[2][3] + projView[2][[2]
	projView[3][3] + projView[3][[2])
};

// Plane normalisation: Normalise, cast to vec4, w=0
for (int i = 0; i < 6; i++)
{
	float d = glm::length(m_frustrumPlanes[[i]);
	m_frustrumPlanes[i] /= d;
}

}

// SET
void Camera::SetProjection(float fov, float aspect, float near, float far)
{
	m_projectionMatrix = glm::perspective(fov, aspect, near, far);
	//UpdateFrustrumPlanes(); 
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
	//m_viewMatrix = glm::lookAt(m_cameraPosition, target, glm::vec3(0, 1, 0));

	// Target position - calculate camera direction
	glm::vec3 dir = glm::normalize(target - m_cameraPosition);
	m_cameraPitch = glm::degrees(glm::asin(dir.y));
	m_cameraYaw = glm::degrees(atan2(dir.y, dir.x));
	
	CalculateFront();
}

bool Camera::IsBoundsInFrustrum(const glm::mat4& modelMatrix, const SphereBoundingVolume & bound)
{
	glm::vec3 actualCentre =
		glm::vec3(modelMatrix * glm::vec4(bounds, GetCentre(), 1));

	glm::vec3 scale;
	glm::quat rot;
	glm::vec3 position;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(modelMatrix, scale, rot, position, skew, perspective);

	float largestScale = (scale.x > scale.y) ? scale.x : scale.y;
	largestScale = (scale.z > largestScale) ? scale.z : largestScale;

	float actualRadius = bounds.GetRadius() * largestScale;

	for (int i = 0; i < 6; ++i)
	{
		// Plane has directoin facing, distance of nearest point of plane from origin of the world
		//   |  \   .	Distance of point and world origin(0,0)
		//   |	 \      Distance of plane
		//  .|_________
		const glm::vec4& plane = m_frustrumPlanes[i];
		float d = glm::dot(glm::vec3(plane), actualCentre) + plane.w;
		if (d < -bounds.GetRadius())
		{
			// behind plane .: outside plane, .: not inside frustrum .: dont render.
			std::cout << "outside of a plane, no need to render !\n";
			return false;
		}
	}
	std::cout << "inside frustum, render"\n;
	return true;
}
