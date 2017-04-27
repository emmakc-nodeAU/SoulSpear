#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "SphereBoundingVolume.h"

class Camera
{
public:
	Camera();
	virtual~Camera() = default;

	virtual void Update(float deltaTime) {}

	// SET: Position
	void setPosition(const glm::vec3& position);

	// GET: Position
	glm::vec3 getPosition() const	{ 
		return glm::vec3(
			m_worldTransform[3].x,  
			m_worldTransform[3].y, 
			m_worldTransform[3].z); }

	// SET: View LookAt
	void lookAt(
		glm::vec3 eye, 
		glm::vec3 centre, 
		glm::vec3 worldUp);

	// SET: Perspective
	void setPerspective(
		float vFOV, 
		float aspectRatio, 
		float nearDistance, 
		float farDistance);

	// SET: Transform
	void setTransform(const glm::mat4 transform) 
	{ 
		m_worldTransform = transform; 
		updateViewFromWorld(); 
	}

	// GET
	glm::mat4 getTransform()	  const { return m_worldTransform; }
	glm::mat4 getView()			  const { return m_viewTransform; }
	glm::mat4 getProjection()	  const { return m_projectionTransform; }
	glm::mat4 getProjectionView() const { return m_projectionTransform * m_viewTransform; }

	// FULSTRUM
	bool isBoundsInFulstrum(const SphereBoundingVolume& bounds);
	
private:
	// World Matrix Inverse
	void updateViewFromWorld();
	// View Matrix Inverse
	void updateWorldFromView();

	// FULSTRUM
	// Will be called to recalculate correct Fustrum Planes
	void updateFustrumPlanes();
	glm::vec4 m_fustrumPlanes[6];

	// CAMERA Transforms
	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
};
