#include "Camera_AIE_ClassicModel.h"

Camera_AIE_ClassicModel::Camera_AIE_ClassicModel()
	: m_worldTransform(1)
	, m_projectionTransform(1)
{
	UpdateViewFromWorld();
}

void Camera_AIE_ClassicModel::SetPosition(const glm::vec3 & position)
{
	m_worldTransform[3] = glm::vec4(position, 1);
	UpdateViewFromWorld();
}

void Camera_AIE_ClassicModel::LookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 worldUp)
{
	m_viewTransform = glm::lookAt(eye, centre, worldUp);
	UpdateWorldFromView();
}

void Camera_AIE_ClassicModel::SetPerspective(float vFOV, float aspectRatio, float nearDistance, float farDistance)
{
	m_projectionTransform = glm::perspective(
		vFOV, aspectRatio, nearDistance, farDistance);
}

void Camera_AIE_ClassicModel::UpdateViewFromWorld()
{
	m_viewTransform = glm::inverse(m_worldTransform);
}

void Camera_AIE_ClassicModel::UpdateWorldFromView()
{
	m_worldTransform = glm::inverse(m_viewTransform);
}
