#include "ParticlesEmitter.h"
#include <gl_core_4_4.h>
#include <glfw3.h>

ParticlesEmitter::ParticlesEmitter()
	: m_particles(nullptr)
	, m_firstDead(0)
	, m_maxParticles(0)
	, m_position(0)
	, m_vao(0), m_vbo(0), m_ibo(0)
	, m_vertexData(nullptr)
{
}

ParticlesEmitter::~ParticlesEmitter()
{
	delete[] m_particles;
	delete[] m_vertexData;

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void ParticlesEmitter::update(float dt, const glm::mat4& a_camTransform)
{
	using glm::vec3;
	using glm::vec4;

	// Spawn Particles
	m_emitTimer += dt;
	while (m_emitTimer > m_emitRate)
	{
		emit();
		m_emitTimer -= m_emitRate;
	}

	unsigned int quad = 0;

	float temp = m_firstDead;
	// update particles and turn live particles into billboard quads
	for (unsigned int i = 0; i < temp; i++)
	{
		Particle* particle = &m_particles[i];
		particle->lifeTime += dt;
		if (particle->lifeTime >= particle->lifeSpan)
		{
			// Swap last alive with this:
			*particle = m_particles[m_firstDead - 1];
			m_firstDead--;
		}
		else
		{
			// Move particle 
			particle->position += particle->velocity * dt;

			// size particle
			particle->size = glm::mix(m_startSize, m_endSize,
				particle->lifeTime / particle->lifeSpan);

			// colour particle
			particle->colour = glm::mix(m_startColour, m_endColour,
				particle->lifeTime / particle->lifeSpan);

			// make a quad the correct size + colour
			float halfSize = particle->size * 0.5f;

			m_vertexData[quad * 4 + 0].position = glm::vec4(halfSize,
				halfSize, 0, 1);

			m_vertexData[quad * 4 + 0].colour = particle->colour;

			m_vertexData[quad * 4 + 1].position = glm::vec4(-halfSize,
				halfSize, 0, 1);

			m_vertexData[quad * 4 + 1].colour = particle->colour;


			m_vertexData[quad * 4 + 2].position = glm::vec4(-halfSize,
				-halfSize, 0, 1);

			m_vertexData[quad * 4 + 2].colour = particle->colour;

			m_vertexData[quad * 4 + 3].position = glm::vec4(halfSize,
				-halfSize, 0, 1);

			m_vertexData[quad * 4 + 3].colour = particle->colour;

			// Create billboard transform
			vec3 zAxis = glm::normalize(vec3(a_camTransform[3]) - particle->position);
			vec3 xAxis = glm::cross(glm::vec3(a_camTransform[1]), zAxis);
			vec3 yAxis = glm::cross(zAxis, xAxis);
			glm::mat4 billboard(
				glm::vec4(xAxis, 0),
				glm::vec4(yAxis, 0),
				glm::vec4(zAxis, 0),
				glm::vec4(0, 0, 0, 1));

			m_vertexData[quad * 4 + 0].position = billboard * m_vertexData[quad * 4 + 0].position +
				glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 1].position = billboard * m_vertexData[quad * 4 + 1].position +
				glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 2].position = billboard * m_vertexData[quad * 4 + 2].position +
				glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 3].position = billboard * m_vertexData[quad * 4 + 3].position +
				glm::vec4(particle->position, 0);

			++quad;
		}
	}
}

void ParticlesEmitter::draw()
{
	// sync the particle vertex buffer
	// based on how many alive particles there are.

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_firstDead * 4 * sizeof(ParticleVertex), m_vertexData);

	// draw particles
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_firstDead * 6, GL_UNSIGNED_INT, 0);
}


void ParticlesEmitter::initalise(unsigned int a_maxParticles, unsigned int e_emitRate,
	float a_lifetimeMin, float a_lifetimeMax,
	float a_velocityMin, float a_velocityMax,
	float a_startSize, float a_endSize,
	const glm::vec4 & a_startColour, const glm::vec4 & a_endColour)
{
	// Set up emit timers.
	m_emitTimer = 0;
	m_emitRate = 1.0f / e_emitRate;

	// Store al lvariables passed in.
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;
	m_lifespanMin = a_lifetimeMin,
		m_lifespanMax = a_lifetimeMax;
	m_maxParticles = a_maxParticles;

	// create particle array
	m_particles = new Particle[m_maxParticles];

	// create the array of vertices for the particles
	// 4 vertices per particle for a quad.
	// will be filled during update.
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	// create the index buffer data for the particles
	// 6 indices per quad of 2 triangles.
	// fill it now as it never changes.

	unsigned int* indexData = new unsigned int[m_maxParticles * 6];
	for (unsigned int i = 0; i < m_maxParticles; ++i)
	{
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;

		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	// create opengl buffers.
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 *
		sizeof(ParticleVertex), m_vertexData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 *
		sizeof(unsigned int), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // colour

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
		sizeof(ParticleVertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;
}

void ParticlesEmitter::movePosition(glm::vec3 &move)
{
	m_position = move;
}

void ParticlesEmitter::emit()
{
	// Only emit if there is a dead particle to use
	if (m_firstDead >= m_maxParticles)
	{
		return;
	}

	// Ressurect the first dead particle
	Particle& particle = m_particles[m_firstDead++];

	// Assign it's starting point
	particle.position = m_position;

	// Randomise it's lifespan
	particle.lifeTime = 0;
	particle.lifeSpan = (rand() / (float)RAND_MAX) *
		(m_lifespanMax - m_lifespanMin) + m_lifespanMin;

	// Set starting size and colour
	particle.colour = m_startColour;
	particle.size = m_startSize;

	// Randmise velocity direction and strength
	float velocity = (rand() / (float)RAND_MAX) *
		(m_velocityMax - m_velocityMin) + m_velocityMin;

	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity = glm::normalize(particle.velocity) * velocity;
}