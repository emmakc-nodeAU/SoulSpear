#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 colour;

	float size;
	float lifeTime;
	float lifeSpan;
};

struct ParticleVertex {
	glm::vec4 position;
	glm::vec4 colour;
};

class ParticlesEmitter
{
public:
	ParticlesEmitter();
	virtual ~ParticlesEmitter();

	void update(float dt, const glm::mat4& a_camTransform);

	void draw();

	void initalise(unsigned int a_maxParticles, unsigned int e_emitRate,
		float a_lifetimeMin, float a_lifetimeMax,
		float a_velocityMin, float a_velocityMax,
		float a_startSize, float a_endSize,
		const glm::vec4& a_startColour, const glm::vec4& a_endColour);

	void movePosition(glm::vec3 &move);

protected:
	Particle* m_particles;
	unsigned int m_firstDead;
	unsigned int m_maxParticles;

	unsigned int m_vao, m_vbo, m_ibo;
	ParticleVertex* m_vertexData;

	glm::vec3 m_position;
	float m_emitTimer;
	float m_emitRate;

	float m_lifespanMin;
	float m_lifespanMax;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	glm::vec4 m_startColour;
	glm::vec4 m_endColour;

	void emit();

private:
};
