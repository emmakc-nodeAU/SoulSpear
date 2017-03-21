#pragma once

#include <functional>
class Shader
{
public:
	Shader();
	virtual ~Shader();		// inherit from shader class

	void Enable();
	void Disable();

	void Load(const char *vertexShader, const char *fragmentShader, std::function<void(unsigned int program)> cbBindAttribs);
	void LoadFile(const char *vertexShaderFile, const char *fragmentShaderFile, std::function<void(unsigned int program)> cbBindAttribs);

	void Unload();

	unsigned int GetProgramId();	// Returns shader program m_shader


protected:
	unsigned int m_shader;
private:

};