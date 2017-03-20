#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

#include <glm/vec3.hpp>

class Camera;

class FlyCameraApp : public aie::Application {
public:

	FlyCameraApp();
	virtual ~FlyCameraApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void DrawGizmosGrid();

protected:

	Camera *m_camera;
};