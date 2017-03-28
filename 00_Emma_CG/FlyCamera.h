#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "Camera.h"

struct GLFWwindow;
class FlyCamera : public Camera
{
public:
	FlyCamera(GLFWwindow* window, float speed);
	~FlyCamera() = default;

	float getSpeed() const { return m_speed; }
	void Update(float deltaTime) override;

protected:
	//void mouseInput(double dt);
	//void getDirectionRotation(double dt, double xPos, double yPos);
	//double m_cursorX, m_cursorY;

private:
	float m_speed;
	float m_rotate;
	GLFWwindow* m_window;
};
