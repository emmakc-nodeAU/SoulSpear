#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>

class Camera
{
public:
	Camera();
	virtual~Camera();

	virtual void Update(float deltaTime);


	// POSITION		----------------------------
	void setPosition(const glm::vec3& pos);
	glm::vec3 getPosition() const {
		return glm::vec3(
			m_worldTransform[3].x,
			m_worldTransform[3].y,
			m_worldTransform[3].z);
	}

	// PERSPECTIVE	----------------------------
	void lookAt(
		glm::vec3 eye,
		glm::vec3 center,
		glm::vec3 worldUp);

	void setPerspective(
		float vFOV,
		float aspectRatio,
		float nearDistance,
		float farDistance);

	// TRANSFORM	----------------------------
	void setTransform(const glm::mat4 transform)
	{
		m_worldTransform = transform;
		updateViewFromWorld();
	}

	glm::mat4 getTransform() const { return m_worldTransform; }
	glm::mat4 getView() { return m_viewTransform; }
	glm::mat4 getProjection() { return m_projectionTransform; }
	glm::mat4 getProjectionView()
	{
		return m_projectionTransform * m_viewTransform;
	}

protected:
	void updateViewFromWorld();
	void updateWorldFromView();

	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
private:
};
