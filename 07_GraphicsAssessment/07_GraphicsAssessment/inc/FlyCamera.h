#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "Camera_AIE_ClassicModel.h"

struct GLFWwindow;

class FlyCamera : public Camera_AIE_ClassicModel
{
public:
	FlyCamera(GLFWwindow* window, float speed);

	float getSpeed() const { return m_speed; }
	~FlyCamera() = default;

	void Update(float deltaTime) override;

protected:
	void mouseInput(double deltaTime);
	void getDirectionRotation(double deltaTime, double xPos, double yPos);
	double m_cursorX, m_cursorY;

private:
	float m_speed;
	float m_rotate;

	// bool m_bViewButtonClicked;

	GLFWwindow* m_window;
};
