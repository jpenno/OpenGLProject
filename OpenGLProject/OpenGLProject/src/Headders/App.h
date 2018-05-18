#pragma once
#include <glm/mat4x4.hpp>
#include "Camera.h"

struct GLFWwindow;

class App
{
public:
	App();
	virtual ~App();

	void run(const char* title, int width, int height);

	bool Startup();
	void update(float deltaTime);
	void draw();
	void shutdown();
	void clearScreen();
	void quit() { m_gameOver = true; }

protected:
	bool m_gameOver;

private:
	bool createWindow(const char* title, int width, int height);

	GLFWwindow * window;
	glm::mat4 view;
	glm::mat4 projection;

	Camera m_camera;

};

