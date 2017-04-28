#include "GPUParticleEmitter.h"
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include "Shader.h"
#include <iostream>

GPUparticleEmitter::GPUparticleEmitter()
	: m_particles(nullptr), m_maxParticles(0),
	m_position(0, 0, 0),
	m_drawShader(0),
	m_updateShader(0),
	m_lastDrawTime(0)
{
	m_vao[0] = 0;
	m_vao[1] = 0;
	m_vbo[0] = 0;
	m_vbo[1] = 0;
}

GPUparticleEmitter::~GPUparticleEmitter()
{
	delete[] m_particles;

	glDeleteVertexArrays(2, m_vao);
	glDeleteBuffers(2, m_vbo);

	// Delete SHaders
	glDeleteProgram(m_drawShader);
	glDeleteProgram(m_updateShader);
}

void GPUparticleEmitter::initalise(unsigned int a_maxParticles, float a_lifespanMin, float a_lifespanMax, float a_velocityMin, float a_velocityMax, float a_startSize, float a_endSize, const glm::vec4 & a_startColour, const glm::vec4 & a_endColour)
{
	// Store all variables.
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;
	m_lifespanMin = a_lifespanMin;
	m_lifespanMax = a_lifespanMax;
	m_maxParticles = a_maxParticles;

	// create particle array
	m_particles = new GPUparticle[a_maxParticles];

	// set our starting ping-pong buffer
	m_activeBuffer = 0;

	createBuffers();
	createUpdateShader();
	createDrawShader();
}

void GPUparticleEmitter::draw(float time, const glm::mat4 & a_cameraTransform, const glm::mat4 a_projectionView)
{
	// update the particles using transform feedback
	glUseProgram(m_updateShader);

	// bind time information
	int loc = glGetUniformLocation(m_updateShader, "time");
	glUniform1f(loc, time);

	float dt = time - m_lastDrawTime;
	m_lastDrawTime = time;

	loc = glGetUniformLocation(m_updateShader, "dt");
	glUniform1f(loc, dt);

	// bubd emitter's pos
	loc = glGetUniformLocation(m_updateShader,
		"emitterPosition");
	glUniform3fv(loc, 1, &m_position[0]);

	// disable rasterisation
	glEnable(GL_RASTERIZER_DISCARD);

	// bind the buffer we will update
	glBindVertexArray(m_vao[m_activeBuffer]);

	// work out the "other" buffer
	unsigned int otherBuffer = (m_activeBuffer + 1) % 2;

	// bind the buffer we will update into as points
	// and begin transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	// disable trabsfirn feedback and enable rasterization again
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	// draw the particles using the Geometry shader to billboard them
	glUseProgram(m_drawShader);
	loc = glGetUniformLocation(m_drawShader, "projectionView");
	glUniformMatrix4fv(loc, 1, false, &a_projectionView[0][0]);

	loc = glGetUniformLocation(m_drawShader, "cameraTransform");
	glUniformMatrix4fv(loc, 1, false, &a_cameraTransform[0][0]);

	// Draw particles in "other" buffer
	glBindVertexArray(m_vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	// swap for next frame
	m_activeBuffer = otherBuffer;
}

void GPUparticleEmitter::movePosition(glm::vec3 & move)
{
	m_position = move;
}

void GPUparticleEmitter::createBuffers()
{
	// Create opengl buffer
	glGenVertexArrays(2, m_vao);
	glGenBuffers(2, m_vbo);

	// setup first buffer
	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(GPUparticle), m_particles, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Velocity
	glEnableVertexAttribArray(2); // Lifetime
	glEnableVertexAttribArray(3); // lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUparticle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPUparticle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUparticle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPUparticle), ((char*)0) + 28);

	// setup the second buffer
	glBindVertexArray(m_vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(GPUparticle), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Velocity
	glEnableVertexAttribArray(2); // Lifetime
	glEnableVertexAttribArray(3); // lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUparticle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPUparticle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUparticle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPUparticle), ((char*)0) + 28);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUparticleEmitter::createUpdateShader()
{
	// create a vertex shader handle
	unsigned int vs = Shader::LoadShader(GL_VERTEX_SHADER, "./shaders/GPUparticle.vert");
	m_updateShader = glCreateProgram();
	glAttachShader(m_updateShader, vs);

	// specify the data that will stream back
	const char* varyings[] = { "position","velocity", "lifetime", "lifespan" };
	glTransformFeedbackVaryings(m_updateShader, 4, varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(m_updateShader);

	// remove unneeded handles
	glDeleteShader(vs);

	// bind the sahder so that we can set some
	// uniforms that don't change perframe
	glUseProgram(m_updateShader);

	// bind lifetime min and max
	int loc = glGetUniformLocation(m_updateShader, "lifeMin");
	glUniform1f(loc, m_lifespanMin);
	loc = glGetUniformLocation(m_updateShader, "lifeMax");
	glUniform1f(loc, m_lifespanMax);
}

void GPUparticleEmitter::createDrawShader()
{
	// load shaders from files and turn into GL shader handles

	unsigned int vs = Shader::LoadShader(GL_VERTEX_SHADER,
		"./shaders/GPUparticle.vert");
	unsigned int gs = Shader::LoadShader(GL_GEOMETRY_SHADER,
		"./shaders/GPUparticle.geometry");
	unsigned int fs = Shader::LoadShader(GL_FRAGMENT_SHADER,
		"./shaders/GPUparticle.frag");

	m_drawShader = glCreateProgram();
	glAttachShader(m_drawShader, vs);
	glAttachShader(m_drawShader, fs);
	glAttachShader(m_drawShader, gs);
	glLinkProgram(m_drawShader);

	int success = 0;

	glGetProgramiv(m_drawShader, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
	{
		int infoLogLength;

		glGetProgramiv(m_drawShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_drawShader, infoLogLength, 0, infoLog);
		std::cout << "Error trying to link shaders: \n";

		std::cout << infoLog << '\n';

		assert(false && "Shader failed to compile correctly.");

		delete[] infoLog;
	}
	// remove unneeded handles
	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);

	// BIND SHADER: Set Uniforms
	glUseProgram(m_drawShader);

	// BIND: Size information for interpolation that won't change.
	int loc = glGetUniformLocation(m_drawShader, "sizeStart");
	glUniform1f(loc, m_startSize);
	loc = glGetUniformLocation(m_drawShader, "sizeEnd");
	glUniform1f(loc, m_endSize);

	// BIND: Colour information for interpolation that won't change.
	loc = glGetUniformLocation(m_drawShader, "colourStart");
	glUniform4fv(loc, 1, &m_startColour[0]);
	loc = glGetUniformLocation(m_drawShader, "colourEnd");
	glUniform4fv(loc, 1, &m_endColour[0]);
}
