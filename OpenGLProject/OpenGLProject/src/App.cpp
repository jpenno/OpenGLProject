#include "App.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Gizmos.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "Cube.h"
#include "SkyBox.h"
#include "OBJMesh\OBJMesh.h"

#include "Input.h"

#include "VertexArray.h"

//tsuemea
using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

App::App()
	:m_gameOver(false) {
}

App::~App() {
}

bool App::createWindow(const char * title, int width, int height) {
	// init glfw
	if (glfwInit() == false)
		return false;

	// Create a windowed mode window and its OpenGL context 
	window = glfwCreateWindow(width, height,
		title,
		nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		return false;
	}

	// Make the window's context current 
	glfwMakeContextCurrent(window);

	// sets vsync
	//glfwSwapInterval(1);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	// setting clear color
	glClearColor(0.25f, 0.25f, 0.25f, 1);

	glEnable(GL_DEPTH_TEST);

	// enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// getting the version of openGL
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	// start input manager
	Input::create();

	return true;
}

bool App::Startup() {
	Gizmos::create(10000, 10000, 10000, 10000);

	m_camera.setPos(vec3(10, 10, 10));
	m_camera.setProjectionView(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
	m_camera.Lookat(vec3(0, 0, 0));
	m_gameOver = false;

	// set the light
	m_light.diffuse = { 1, 1, 0 };
	m_light.specular = { 1, 1, 1 };
	//m_light.specular = { 0, 0, 1 };

	//m_ambientLight = { 0.25f, 0.25f, 0.25f };
	m_ambientLight = { 0, 0, 1 };

	return true;
}

void App::run(const char* title, int width, int height )
{
	if (createWindow(title,  width,  height) &&
		Startup()) 
	{
		// variables for timing
		float prevTime = (float)glfwGetTime();
		float currTime = 0;
		float deltaTime = 0;
		unsigned int frames = 0;
		float fpsInterval = 0;
	
		// scope to destory vertex array object before closing the openGL window
		{
			GLCall(glEnable(GL_BLEND));
			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

			Shader objShader("data/shaders/objShader.shader");

			aie::OBJMesh mesh;
			mesh.load("data/soulspear/soulspear.obj", true, true);

			glm::mat4 m_bunnyTransform;
			m_bunnyTransform = {
				0.5f,0,0,0,
				0,0.5f,0,0,
				0,0,0.5f,0,
				0,1,0,1
			};

			//Mesh mesh(glm::vec3(0.0f, 0.0f, 0.0f), m_camera.GetProjectionView());


			std::vector<std::string> fielPaths;
			fielPaths.emplace_back("data/Textures/lilacisles_ft.png");
			fielPaths.emplace_back("data/Textures/lilacisles_bk.png");
			fielPaths.emplace_back("data/Textures/lilacisles_up.png");
			fielPaths.emplace_back("data/Textures/lilacisles_dn.png");
			fielPaths.emplace_back("data/Textures/lilacisles_rt.png");
			fielPaths.emplace_back("data/Textures/lilacisles_lf.png");

			//SkyBox skyBox(fielPaths, m_camera.GetProjectionView(), m_camera.GetViewMat());

			Cube Testcube (fielPaths, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), m_camera.GetProjectionView());
			Cube Testcube2(fielPaths, glm::vec4(2.0f, 0.0f, 0.0f, 1.0f), m_camera.GetProjectionView());
			Cube Testcube4(fielPaths, glm::vec4(6.0f, 0.0f, 0.0f, 1.0f), m_camera.GetProjectionView());

			while (!m_gameOver)
			{
				// update delta time
				currTime = (float)glfwGetTime();
				deltaTime = currTime - prevTime;
				prevTime = currTime;
				// rotate light
				m_light.direction = glm::normalize(vec3(glm::cos(currTime * 2),
					glm::sin(currTime * 2), 0));
				//m_light.direction = glm::normalize(vec3(0, -1, 0));

				// clear input
				Input::getInstance()->clearStatus();

				// update window events (input etc)
				glfwPollEvents();

				update(deltaTime);

				// clear the screen
				clearScreen();

				Testcube.Draw(m_camera.GetProjectionView());
				Testcube2.Draw(m_camera.GetProjectionView());
				Testcube4.Draw(m_camera.GetProjectionView());

				// draw obj
				objShader.Bind();
				// bind transform
				auto pvm = m_camera.GetProjectionView() * m_bunnyTransform;
				objShader.SetuniformMat4f("ProjectionViewModel", pvm);
				objShader.SetuniformMat3f("NormalMatrix",
					glm::inverseTranspose(glm::mat3(m_bunnyTransform)));

				objShader.SetuniformMat4f("ModelMatrix", m_bunnyTransform);

				// bind light
				objShader.SetUniform3f("Ia", m_ambientLight);
				objShader.SetUniform3f("Id", m_light.diffuse);
				objShader.SetUniform3f("Is", m_light.specular);
				objShader.SetUniform3f("LightDirection", m_light.direction);
				objShader.SetUniform3f("cameraPosition", m_camera.GetPos());
				objShader.SetUniform1f("specularPower", 125.0f);
				// set metiral 
				objShader.SetUniform3f("Ka", glm::vec3(1, 1, 1));	// ambient material colour
				objShader.SetUniform3f("Kd", glm::vec3(0.25f, 0.25f, 0.25f));	// diffuse material colour
				objShader.SetUniform3f("Ks", glm::vec3(0.25f, 0.25f, 0.25f));	// specular material colour


				mesh.draw();

				//skyBox.Draw(m_camera.GetViewMat());

				draw();

				if (glfwWindowShouldClose(window) == GLFW_TRUE)
					quit();
			}
		}
		shutdown();
	}
}

void App::update(float deltaTime) 
{
	Input* input = Input::getInstance();
	if (input->isKeyDown(INPUT_KEY_ESCAPE))
		quit();

	m_camera.update(deltaTime);
}

void App::draw() 
{
	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1));

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i) 
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);

		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	Gizmos::draw(m_camera.GetProjectionView());
	glfwSwapBuffers(window);
}

void App::shutdown()
{
	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	Input::destory();
}

void App::clearScreen() 
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

