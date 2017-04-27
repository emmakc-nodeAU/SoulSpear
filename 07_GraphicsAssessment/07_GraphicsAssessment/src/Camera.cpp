#include "Camera.h"
#include <iostream>

Camera::Camera()
	: m_worldTransform(1)
	, m_projectionTransform(1)
{
	updateViewFromWorld();
}

void Camera::setPosition(const glm::vec3 & position)
{
	m_worldTransform[3] = glm::vec4(position, 1);
	updateViewFromWorld();
}

void Camera::lookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 worldUp)
{
	m_viewTransform = glm::lookAt(eye, centre, worldUp);
	updateWorldFromView();
}

void Camera::setPerspective(float vFOV, float aspectRatio, float nearDistance, float farDistance)
{
	m_projectionTransform = glm::perspective(
		vFOV, aspectRatio, nearDistance, farDistance);
	updateFustrumPlanes();
}

bool Camera::isBoundsInFulstrum(const SphereBoundingVolume & bounds)
{
	for (int i = 0; i < 6; ++i)
	{
		const glm::vec4& plane = m_fustrumPlanes[i];
		float d = glm::dot(glm::vec3(plane), bounds.GetCentre()) + plane.w;
		if (d < -bounds.GetRadius())
		{
			std::cout << "Outside of a plane, no need to render\n";
			return false;
		}
	}

	std::cout << "Inside the bounds!\n";
	return true;
}

	void Camera::updateViewFromWorld()
{
	m_viewTransform = glm::inverse(m_worldTransform);
	updateFustrumPlanes();
}

void Camera::updateWorldFromView()
{
	m_worldTransform = glm::inverse(m_viewTransform);
	updateFustrumPlanes();
}

void Camera::updateFustrumPlanes()
{
	glm::mat4& projView = getProjectionView();

	// RIGHT
	m_fustrumPlanes[0] = glm::vec4(
		projView[0][3] - projView[0][0],
		projView[1][3] - projView[1][0],
		projView[2][3] - projView[2][0],
		projView[3][3] - projView[3][0]);
	// LEFT
	m_fustrumPlanes[1] = glm::vec4(
		projView[0][3] + projView[0][0],
		projView[1][3] + projView[1][0],
		projView[2][3] + projView[2][0],
		projView[3][3] + projView[3][0]);
	// TOP
	m_fustrumPlanes[2] = glm::vec4(
		projView[0][3] - projView[0][1],
		projView[1][3] - projView[1][1],
		projView[2][3] - projView[2][1],
		projView[3][3] - projView[3][1]);
	// BASE
	m_fustrumPlanes[3] = glm::vec4(
		projView[0][3] + projView[0][1],
		projView[1][3] + projView[1][1],
		projView[2][3] + projView[2][1],
		projView[3][3] + projView[3][1]);
	// FAR
	m_fustrumPlanes[4] = glm::vec4(
		projView[0][3] - projView[0][2],
		projView[1][3] - projView[1][2],
		projView[2][3] - projView[2][2],
		projView[3][3] - projView[3][2]);
	// NEAR
	m_fustrumPlanes[5] = glm::vec4(
		projView[0][3] + projView[0][2],
		projView[1][3] + projView[1][2],
		projView[2][3] + projView[2][2],
		projView[3][3] + projView[3][2]);

	// PLANE NORMALIZATION
	for (int i = 0; i < 6; ++i)
	{
		float d = glm::length(glm::vec3(m_fustrumPlanes[i]));
		m_fustrumPlanes[i] /= d;
	}
}
