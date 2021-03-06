#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include <vector>

class RenderData;
class Shader;

class _01_ProjectSetupApp : public aie::Application {
public:

	_01_ProjectSetupApp();
	virtual ~_01_ProjectSetupApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	std::vector<RenderData*> m_SoulSpear;

	Shader* m_SoulSpearShader;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
};