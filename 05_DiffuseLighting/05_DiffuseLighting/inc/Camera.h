#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
public:
	Camera();
	virtual~Camera();

	virtual void Update(float deltaTime);	// inherit from CameraClass for other camera types

	// SET
	void SetPosition(const glm::vec3 &pos);
	void SetYawPitchRoll(float yaw, float pitch, float roll);
	void SetProjection(float fov, float aspect, float near, float far);


	// GET
	glm::vec3& GetPosition();
	float GetYaw();
	float GetPitch();
	float GetRoll();

	glm::mat4& GetView();
	glm::mat4& GetProjection();

	void Lookat(glm::vec3 target);

protected:

	void CalculateFront();

	// MOVEMENT
	glm::vec3 m_cameraPosition = glm::vec3(0, 2, -2);	// 
	glm::vec3 m_cameraFront = glm::vec3(0, 0, 1);
	glm::vec3 m_cameraUp = glm::vec3(0, 1, 0);

	float m_cameraYaw = 0.0f;	// Rotation around Y axis = upright L/R 
	float m_cameraPitch = 0.0f;	// Rotation around X axis = lean up/down
	float m_cameraRoll = 0.0f;	// Rotation around Z axis = banking angles

	float m_cameraMoveSpeed = 10.0f;

	// Mouse
	int m_lastMouseXPos, m_lastMouseYPos;

	// camera transforms
	glm::mat4	m_viewMatrix;			// Position/ Rotation/ Scale for CAMERA
	glm::mat4	m_projectionMatrix;


private:

	// Scene management - Calculates correct frustrum planes
	// void UpdateFrustrumPlanes();

	// Array to hold our calculate frunstum planes
	// glm::vec4 m_frustrumPlanes[6];
};

/*
// IN CAMERA CPP
// UpdateFrustrumPlanes - inside anywhere that updates matrix projection/view

// Frustrum Cullings
void Camera::UpdateFrustrumPlanes()
{
glm::mat4& projView = GetProjectionView();
// Right Side
m_frustrumPlanes[0] = glm::vec4 (
projView[0][3] - projView[0][[0]
projView[1][3] - projView[1][[0]
projView[2][3] - projView[2][[0]
projView[3][3] - projView[3][[0]

// Left Side
m_frustrumPlanes[1] = glm::vec4 (
projView[0][3] + projView[0][[0]
projView[1][3] + projView[1][[0]
projView[2][3] + projView[2][[0]
projView[3][3] + projView[3][[0]

// Top Side
m_frustrumPlanes[2] = glm::vec4 (
projView[0][3] - projView[0][[1]
projView[1][3] - projView[1][[1]
projView[2][3] - projView[2][[1]
projView[3][3] - projView[3][[1]

// Bottom Side
m_frustrumPlanes[3] = glm::vec4 (
projView[0][3] + projView[0][[1]
projView[1][3] + projView[1][[1]
projView[2][3] + projView[2][[1]
projView[3][3] + projView[3][[1]

// Far Side
m_frustrumPlanes[4] = glm::vec4 (
projView[0][3] - projView[0][[2]
projView[1][3] - projView[1][[2]
projView[2][3] - projView[2][[2]
projView[3][3] - projView[3][[2]

// Near Side
m_frustrumPlanes[5] = glm::vec4 (
projView[0][3] + projView[0][[2]
projView[1][3] + projView[1][[2]
projView[2][3] + projView[2][[2]
projView[3][3] + projView[3][[2]
};

// Plane normalisation: Normalise, case back to vec4 using 0 for w.
for (int i = 0; i < 6; i++)
	{
	float d = glm::length(m_frustrumPlanes[[i]);
	m_frustrumPlanes[i] /= d;
	}


// Sphere Bounds
// RenderData - generate bounding volume too
create a new class: sphere bounding volume
*/
