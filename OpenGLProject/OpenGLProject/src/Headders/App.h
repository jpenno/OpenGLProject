#pragma once
#include <glm/mat4x4.hpp>
#include "Camera.h"
#include "Lights.h"
#include <string>

struct GLFWwindow;
namespace aie {
	class OBJMesh;
}
class Shader;
class Cube;
class SkyBox;

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

	// shaders
	Shader * m_lightingShader = nullptr;

	// obj
	glm::mat4 soulspherPos;
	aie::OBJMesh * m_soulsper = nullptr;
	glm::mat4 nanosuitPos;
	aie::OBJMesh * m_nanosuitMesh = nullptr;



	Cube * m_cube = nullptr;

	SkyBox * m_skyBox = nullptr;

	// lighting 
	DirLight m_dirLight;
	PointLight m_pointLight;


	// 
	GLFWwindow * window;
	glm::mat4 view;
	glm::mat4 projection;

	Camera m_camera;
};

