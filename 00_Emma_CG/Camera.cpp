#include "Camera.h"

Camera::Camera()
	: m_worldTransform(1)
	, m_projectionTransform(1)
{
	updateViewFromWorld();
}

void Camera::setPosition(const glm::vec3 & pos)
{
	m_worldTransform[3] = glm::vec4(pos, 1);
	updateViewFromWorld();
}

void Camera::lookAt(
	glm::vec3 eye, 
	glm::vec3 center, 
	glm::vec3 worldUp)
{
	m_viewTransform = glm::lookAt(
		eye, 
		center, 
		worldUp);
	updateWorldFromView();
}

void Camera::setPerspective(
	float vFOV, 
	float aspectRatio, 
	float nearDistance, 
	float farDistance)
{
	m_projectionTransform = glm::perspective(
		vFOV, 
		aspectRatio, 
		nearDistance, 
		farDistance);
}

void Camera::updateViewFromWorld()
{
	m_viewTransform = glm::inverse(m_worldTransform);
}

void Camera::updateWorldFromView()
{
	m_worldTransform = glm::inverse(m_viewTransform);
}