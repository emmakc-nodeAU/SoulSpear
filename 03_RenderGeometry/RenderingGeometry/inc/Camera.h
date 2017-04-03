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
};
