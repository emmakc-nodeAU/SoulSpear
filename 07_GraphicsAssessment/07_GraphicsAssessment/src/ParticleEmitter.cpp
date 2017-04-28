#include "ParticleEmitter.h"
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

ParticleEmitter::ParticleEmitter() 
	: m_particles(nullptr),
	m_firstDead(0),
	m_maxParticles(0),
	m_position(0, 0, 0),
	m_vao(0), m_vbo(0), m_ibo(0),
	m_vertexData(nullptr) 
{
}

ParticleEmitter::~ParticleEmitter()
{
	delete[] m_particles;
	delete[] m_vertexData;

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void ParticleEmitter::update(float a_deltaTime, const glm::mat4& a_cameraTransform)
{
	using glm::vec3;
	using glm::vec4;

	// Spawn Particles
	m_emitTimer += a_deltaTime;
	while (m_emitTimer > m_emitRate)
	{
		emit();
		m_emitTimer -= m_emitRate;
	}
	unsigned int quad = 0;
	float temp = m_firstDead;

	// Update particles and turn live particles into billboard quads
	for (unsigned int i = 0; i < temp; i++)
	{
		Particle* particle = &m_particles[i];
		particle->lifeTime += a_deltaTime;
		if (particle->lifeTime >= particle->lifeSpan)
		{
			// Swap last alive with this one
			*particle = m_particles[m_firstDead - 1];
			m_firstDead--;
		}
		else
		{
			// Move Particle
			particle->position += particle->velocity * a_deltaTime;

			// Size particle
			particle->size = glm::mix(m_startSize, m_endSize,
				particle->lifeTime / particle->lifeSpan);

			// Colour particle
			particle->colour = glm::mix(m_startColour, m_endColour,
				particle->lifeTime / particle->lifeSpan);

			// Make a quad the correct size and colour
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

			// CREATE: Billboard transform
			vec3 zAxis = glm::normalize(vec3(a_cameraTransform[3]) - particle->position);
			vec3 xAxis = glm::cross(vec3(a_cameraTransform[1]), zAxis);
			vec3 yAxis = glm::cross(zAxis, xAxis);
			glm::mat4 billboard(
				glm::vec4(xAxis, 0),
				glm::vec4(yAxis, 0),
				glm::vec4(zAxis, 0),
				glm::vec4(0, 0, 0, 1));

			m_vertexData[quad * 4 + 0].position = billboard *
				m_vertexData[quad * 4 + 0].position +
				glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 1].position = billboard *
				m_vertexData[quad * 4 + 1].position +
				glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 2].position = billboard *
				m_vertexData[quad * 4 + 2].position +
				glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 3].position = billboard *
				m_vertexData[quad * 4 + 3].position +
				glm::vec4(particle->position, 0);

			++quad;
		}
	}
}

void ParticleEmitter::draw()
{
	// SYNC: Particle Vertex Buffer
	// Based on number of alive particles
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_firstDead * 4 *
		sizeof(ParticleVertex), m_vertexData);

	// DRAW: Particles!!!!! YAY
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_firstDead * 6, GL_UNSIGNED_INT, 0);
}

void ParticleEmitter::initalise(unsigned int a_maxParticles,
	unsigned int a_emitRate,
	float a_lifetimeMin, float a_lifetimeMax,
	float a_velocityMin, float a_velocityMax,
	float a_startSize, float a_endSize,
	const glm::vec4 & a_startColour,
	const glm::vec4 & a_endColour)
{
	// SET: Emit Timers
	m_emitTimer = 0;
	m_emitRate = 1.0f / a_emitRate;

	// STORE: Variabled passed in
	m_startColour	= a_startColour;
	m_endColour		= a_endColour;
	m_startSize		= a_startSize;
	m_endSize		= a_endSize;
	m_velocityMin	= a_velocityMin;
	m_velocityMax	= a_velocityMax;
	m_lifespanMin	= a_lifetimeMin;
	m_lifespanMax	= a_lifetimeMax;
	m_maxParticles	= a_maxParticles;

	// CREATE: Particle Array
	m_particles = new Particle[m_maxParticles];
	
	// CREATE:  Vertices Array for the particles
	//			4 vertices per particle for a quad are filled at Update.
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	// CREATE:	Index Buffer Data for the particles
	//			6 indices per Quad of 2 Triangles, filled once
	unsigned int* indexData = new unsigned int[m_maxParticles * 6];
	for (unsigned int i = 0; i < m_maxParticles; ++i) {
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;

		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	// CREATE:	OpenGL Buffers
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 *
		sizeof(ParticleVertex), m_vertexData,
		GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 *
		sizeof(unsigned int), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Colour
								  // POSITION
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(ParticleVertex), 0);
	// COLOUR
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
		sizeof(ParticleVertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;
}

void ParticleEmitter::movePosition(glm::vec3 &move)
{
	m_position = move;
}

void ParticleEmitter::emit()
{
	// EMIT IF DEAD PARTICLE TO USE (Recycling)
	if (m_firstDead >= m_maxParticles)
	{
		return;
	}

	// RESURRECT 1st Dead Particle
	Particle& particle = m_particles[m_firstDead++];

	// ASSIGN: Starting position
	particle.position = m_position;

	// LIFESPAN: Randomise
	particle.lifeTime = 0;
	particle.lifeSpan = (rand() / (float)RAND_MAX) *
		(m_lifespanMax - m_lifespanMin) + m_lifespanMin;

	// SET: Start size, colour
	particle.colour = m_startColour;
	particle.size	= m_startSize;

	// RANDOMISE: Veolcity direction and Strength
	float velocity = (rand() / (float)RAND_MAX) *
		(m_velocityMax - m_velocityMin) + m_velocityMin;
	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity = glm::normalize(particle.velocity) * velocity;
}