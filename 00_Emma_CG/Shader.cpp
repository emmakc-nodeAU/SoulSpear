#include "Shader.h"
#include <gl_core_4_4.h>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <sstream>

Shader::Shader(std::string vertexPath, std::string fragPath)
	: m_programID(-1)
{
	MakeShaderProgram(vertexPath, fragPath);
}

Shader::~Shader()
{
	if (m_programID != -1)
	{
		glDeleteProgram(m_programID);
	}
}

Shader::Shader(Shader && other)
{
	m_programID = other.m_programID;
	other.m_programID = -1;
}

void Shader::MakeShaderProgram(std::string vertexPath, std::string fragPath)
{
	// LOAD shaders from disc
	unsigned int vertexShader	= MakeShader(GL_VERTEX_SHADER, vertexPath);
	unsigned int fragmentShader = MakeShader(GL_FRAGMENT_SHADER, fragPath);
	assert(vertexShader != -1 && fragmentShader != -1);

	// CREATE Shader Program, attach shaders, link shaders
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	int success = 0;
	// IF FAIL - Error check:
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		std::cout << "Error trying to link shaders (" << vertexPath
			<< ", " << fragPath << "): /n";
		std::cout << infoLog << '\n';
		delete[] infoLog;
		assert(false && "Shader failed to compile correctly.");
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

unsigned int Shader::MakeShader(unsigned int type, std::string path)
{
	std::ifstream file;
	file.open(path.c_str(), std::ifstream::in); 
	if (!file.good()) 
	{
		return -1;
	}
	// READ FILE BUFFER and LOAD STRING STREAM
	std::stringstream ss; 
	ss << file.rdbuf(); 
	file.close(); 

	std::string codeString = ss.str(); 
	unsigned int shaderHandle = glCreateShader(type);
	const char* shaderCode = codeString.c_str();

	glShaderSource(shaderHandle, 1, (const char**)&shaderCode, 0);
	glCompileShader(shaderHandle);

	// Return compiled shader for shader program
	return shaderHandle;
}