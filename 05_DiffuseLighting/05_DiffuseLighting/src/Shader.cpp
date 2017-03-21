#include "Shader.h"
#include <gl_core_4_4.h>

#include <string>
#include <fstream>
#include <streambuf>

Shader::Shader()
{

}

Shader::~Shader()		// inherit from shader class
{

}

void Shader::Enable()
{
	glUseProgram(m_shader);
}

void Shader::Disable()
{
	glUseProgram(0);
}

void Shader::Load(const char *vertexShader, const char *fragmentShader, std::function<void(unsigned int program)> cbBindAttribs)
{
	int success = GL_FALSE;	// Error checking 

							// LOAD GEOMETRY
							/* STEP 1:
							Create vertex shader, link to source code, compile it */
							// Create: Vertex Shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShader, NULL);
	glCompileShader(vs);

	/*STEP 2:
	Create fragment shader, link to source code, compile it */
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShader, NULL);
	glCompileShader(fs);

	/* STEP 3:
	Create Shader Program */
	m_shader = glCreateProgram();

	/* STEP 4:
	Attach vs & fs to shader program. */
	glAttachShader(m_shader, vs);
	glAttachShader(m_shader, fs);

	// BINDING HERE
	cbBindAttribs(m_shader);


	// Link: Vert and Frag
	glLinkProgram(m_shader);
	
	// ERROR HELPER
	/*glGetProgramiv(m_shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infolog = new char[infoLogLength];

		glGetProgramInfoLog(m_shader, infoLogLength, 0, infolog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infolog);
		delete[] infolog;
	}*/

	/* STEP 6:
	Destroy vertex and fragment shaders
	They are now combined into shaderProgram*/
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Shader::LoadFile(const char *vertexShaderFile, const char *fragmentShaderFile, std::function<void(unsigned int program)> cbBindAttribs)
{
	std::ifstream vsFile(vertexShaderFile);
	std::string vertexShader((std::istreambuf_iterator<char>(vsFile)), std::istreambuf_iterator<char>());

	std::ifstream fsFile(fragmentShaderFile);
	std::string fragmentShader((std::istreambuf_iterator<char>(fsFile)), std::istreambuf_iterator<char>());

	Load(vertexShader.c_str(), fragmentShader.c_str(), cbBindAttribs);
}

void Shader::Unload()
{
	glDeleteProgram(m_shader);
}

unsigned int Shader::GetProgramId()	// Returns shader program m_shader
{
	return m_shader;
}