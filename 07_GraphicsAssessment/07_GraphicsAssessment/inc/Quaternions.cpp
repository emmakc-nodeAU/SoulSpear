#include "Quaternions.h"
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include <Gizmos.h>
#include <glm/gtx/transform.hpp>

bool Quaternions::startup()
{
	m_positions[0] = glm::vec3(10, 0, 10);
	m_positions[1] = glm::vec3(10, 0, -10);
	m_positions[2] = glm::vec3(-10, 0, -10);
	m_positions[3] = glm::vec3(-10, 0, 10);

	m_rotations[0] = glm::quat(glm::vec3(0, -1, 0));
	m_rotations[1] = glm::quat(glm::vec3(0, 1, 0));
	m_rotations[2] = glm::quat(glm::vec3(0, -1, 0));
	m_rotations[3] = glm::quat(glm::vec3(0, 1, 0));

	m_currentFrame = 0;
	m_nextFrame = 0;
	m_t = 1.0f;

	m_speed = 0.5f;

	return true;
}


void Quaternions::shutdown()
{
}

void Quaternions::update(float dt)
{
	m_t += (dt * m_speed);
	if (m_t >= 1.0f)
	{
		m_t = 0.0f;
		m_currentFrame = m_nextFrame;
		m_nextFrame = (++m_nextFrame) % 4;
	}
	p = (1.0f - m_t) * m_positions[m_currentFrame] + m_t * m_positions[m_nextFrame];

	// Quaternion slerp
	r = glm::slerp(
		m_rotations[m_currentFrame], 
		m_rotations[m_nextFrame], m_t);

	// Build a matrix
	glm::mat4 m = glm::translate(p) * glm::toMat4(r);

	// Draw a transform and box
	aie::Gizmos::addTransform(m);
	aie::Gizmos::addAABBFilled(
		glm::vec3(0, 0, 0), 
		glm::vec3(.5f), 
		glm::vec4(1, 0, 0, 1), &m);
	return;
}

glm::vec3 Quaternions::getPosition()
{
	return p;
}
