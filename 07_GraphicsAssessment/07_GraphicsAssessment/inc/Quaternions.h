#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

class Quaternions
{
public:
	bool startup();
	void shutdown();

	void update(float dt);
	glm::vec3 getPosition();

protected:

private:

	glm::vec3		m_positions[4];
	glm::quat		m_rotations[4];

	unsigned int	m_currentFrame;
	unsigned int	m_nextFrame;
	float			m_t;
	float			m_speed;

	glm::vec3		p;
	glm::quat		r;
};