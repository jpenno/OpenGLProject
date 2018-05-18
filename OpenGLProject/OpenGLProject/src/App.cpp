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


#include "Input.h"

#include "VertexArray.h"

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
	return true;
}

void App::run(const char* title, int width, int height ){
	if (createWindow(title,  width,  height) &&
		Startup()) {
		// variables for timing
		float prevTime = glfwGetTime();
		float currTime = 0;
		float deltaTime = 0;
		unsigned int frames = 0;
		float fpsInterval = 0;
	

		float positions[] = {
			// front
			-0.5f, -0.5f,  0.5f, 0.0, 0.0, // 0
			 0.5f, -0.5f,  0.5f, 1.0, 0.0, // 1
			 0.5f,  0.5f,  0.5f, 1.0, 1.0, // 2
			-0.5f,  0.5f,  0.5f, 0.0, 1.0, // 3

			// Right Face
			 0.5f, -0.5f,  0.5f, 0.0, 0.0, // 4
			 0.5f, -0.5f, -0.5f, 1.0, 0.0, // 5
			 0.5f,  0.5f, -0.5f, 1.0, 1.0, // 6
			 0.5f,  0.5f,  0.5f, 0.0, 1.0, // 7

			 // Left Face
			-0.5f, -0.5f,  0.5f, 0.0, 0.0, // 8
			-0.5f, -0.5f, -0.5f, 1.0, 0.0, // 9
			-0.5f,  0.5f, -0.5f, 1.0, 1.0, // 10
			-0.5f,  0.5f,  0.5f, 0.0, 1.0, // 11

			// Back face
			 0.5f, -0.5f, -0.5f, 1.0, 0.0, // 12
			-0.5f, -0.5f, -0.5f, 0.0, 0.0, // 13
			-0.5f,  0.5f, -0.5f, 0.0, 1.0, // 14
			 0.5f,  0.5f, -0.5f, 1.0, 1.0, // 15

			// Top face
			 0.5f, 0.5f,  0.5f, 1.0, 1.0,// 16
			 0.5f, 0.5f, -0.5f, 1.0, 0.0,// 17
			-0.5f, 0.5f, -0.5f, 0.0, 0.0,// 18
			-0.5f, 0.5f,  0.5f, 0.0, 1.0,// 19

			// Bottom face
			 0.5f,  -0.5f,  0.5f, 1.0, 1.0,// 20
			 0.5f,  -0.5f, -0.5f, 1.0, 0.0,// 21
			-0.5f,  -0.5f, -0.5f, 0.0, 0.0,// 22
			-0.5f,  -0.5f,  0.5f, 0.0, 1.0 // 23
		};

		unsigned int indices[] = {
				0, 1, 2,		0, 2, 3,	// front face
				4, 5, 6,		4, 6, 7,	// Right face
				10, 9, 8,		11, 10, 8,	// Left face 
				12, 13, 14,		12, 14, 15,	// Back face
				16, 17, 18,		16, 18, 19, // Top face
				22, 21, 20,		22, 20, 23	// Bottom face
		};
		// scope to destory vertex array object before closing the openGL window
		{
			GLCall(glEnable(GL_BLEND));
			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			VertexArray va;
			VertexBuffer vb(positions, 24 * 5 * sizeof(float));

			Shader shader("data/shaders/Basic.shader");
			shader.Bind();
			shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

			glm::mat4 orthoProj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
			//shader.SetuniformMat4f("u_MVP", orthoProj);

			VertexBufferLayout layout;
			layout.Push<float>(3);
			layout.Push<float>(2);
			va.AddBuffer(vb, layout);

			IndexBuffer ib(indices, 36);

			Texture texture("data/textures/4-2-car-png-hd.png");
			texture.Bind();
			shader.SetUniform1i("u_Texture", 0);


			Renderer randerer;

			while (!m_gameOver) {
				// update delta time
				currTime = glfwGetTime();
				deltaTime = currTime - prevTime;
				prevTime = currTime;

				// clear input
				Input::getInstance()->clearStatus();

				// update window events (input etc)
				glfwPollEvents();

				update(deltaTime);

				// clear the screen
				clearScreen();

				//shader.Bind();
				shader.SetuniformMat4f("u_MVP", m_camera.GetProjectionView());

				randerer.Draw(va, ib, shader);

				draw();

				if (glfwWindowShouldClose(window) == GLFW_TRUE)
					quit();
			}
		}
		shutdown();
	}
}

void App::update(float deltaTime) {
	Input* input = Input::getInstance();
	if (input->isKeyDown(INPUT_KEY_ESCAPE))
		quit();

	m_camera.update(deltaTime);
}

void App::draw() {

	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1));

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);

		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	//Gizmos::addSphere(vec3(0, 0, 0), 1.0f, 12, 12, vec4(1.0f, 1.0f, 0.0f, 1.0f));

	Gizmos::draw(m_camera.GetProjectionView());
	glfwSwapBuffers(window);
}

void App::shutdown() {
	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	Input::destory();
}

void App::clearScreen() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

