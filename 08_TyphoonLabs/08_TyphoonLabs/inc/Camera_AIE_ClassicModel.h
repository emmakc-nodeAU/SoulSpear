#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "SphereBoundingVolume.h"

class Camera_AIE_ClassicModel
{
public:
	Camera_AIE_ClassicModel();
	virtual~Camera_AIE_ClassicModel() = default;

	virtual void Update(float deltaTime) {}

	// SET: Position
	void SetPosition(const glm::vec3& position);

	// GET: Position
	glm::vec3 GetPosition() const	{ 
		return glm::vec3(
			m_worldTransform[3].x,  
			m_worldTransform[3].y, 
			m_worldTransform[3].z); }

	// SET: View LookAt
	void LookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 worldUp);

	// SET: Perspective
	void SetPerspective(float vFOV, float aspectRatio, float nearDistance, float farDistance);

	// SET: Transform
	void SetTransform(const glm::mat4 trans) { m_worldTransform = trans; UpdateViewFromWorld(); }


	// GET
	glm::mat4 GetTransform() const { return m_worldTransform; }
	glm::mat4 GetView() const { return m_viewTransform; }
	glm::mat4 GetProjection() const { return m_projectionTransform; }
	glm::mat4 GetProjectionView() const { return m_projectionTransform * m_viewTransform; }

	// FULSTRUM
	bool IsBoundsInFulstrum(const SphereBoundingVolume& bounds);

protected:
	
private:
	// World Matrix Inverse
	void UpdateViewFromWorld();

	// View Matrix Inverse
	void UpdateWorldFromView();

	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;

	// FULSTRUM
	// Will be called to recalculate correct Fustrum Planes
	void UpdateFustrumPlanes();
	glm::vec4 m_fustrumPlanes[6];

};
