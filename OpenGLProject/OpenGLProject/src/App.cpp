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

#include "Input.h"

#include "VertexArray.h"
// tmp
#include "stb_image/stb_image.h"


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
	
		// scope to destory vertex array object before closing the openGL window
		{
			GLCall(glEnable(GL_BLEND));
			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

			float skyboxVertices[] = {
				// Positions
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				1.0f, -1.0f,  1.0f
			};

			Shader skyBoxShader("data/shaders/skyBox.shader");
			skyBoxShader.Bind();

			VertexArray skyVa;
			VertexBuffer skyVb(skyboxVertices, sizeof(skyboxVertices));

			VertexBufferLayout skyVaLayout;
			skyVaLayout.Push<float>(3);
			skyVa.AddBuffer(skyVb, skyVaLayout);

			int width, height, BP;


			std::vector<const char*> fielPaths;
			fielPaths.push_back("data/Textures/lilacisles_ft.png");	
			fielPaths.push_back("data/Textures/lilacisles_bk.png");	
			fielPaths.push_back("data/Textures/lilacisles_up.png");	
			fielPaths.push_back("data/Textures/lilacisles_dn.png"); 
			fielPaths.push_back("data/Textures/lilacisles_rt.png"); 
			fielPaths.push_back("data/Textures/lilacisles_lf.png");	


			unsigned int CubeMagTexture;
			GLCall(glGenTextures(1, &CubeMagTexture));

			GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMagTexture));


			for (int i = 0; i < fielPaths.size(); i++) {

				//stbi_set_flip_vertically_on_load(1);
				unsigned char* localBuffer = stbi_load(fielPaths[i], &width, &height, &BP, 4);

				GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8,
									width, height,	0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));

				//GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

				if (localBuffer)
					stbi_image_free(localBuffer);
			}

			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
			GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));


			glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 1000.0f);

			Cube Testcube(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 1.0f);
			Cube Testcube2(glm::vec4(1.0f, 2.0f, 0.0f, 1.0f), 1.0f);
			Cube Testcube4(glm::vec4(1.0f, 3.0f, 0.0f, 1.0f), 1.0f);

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

				Testcube.Draw(m_camera.GetProjectionView());
				Testcube2.Draw(m_camera.GetProjectionView());
				Testcube4.Draw(m_camera.GetProjectionView());

				GLCall(glDepthFunc(GL_LEQUAL));
				skyBoxShader.Bind();
				glm::mat3 viweMat = glm::mat3(m_camera.GetViewMat());
				skyBoxShader.SetuniformMat4f("view", viweMat);
				skyBoxShader.SetuniformMat4f("projection", projection);
				skyVa.Bind();

				glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMagTexture);

				GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
				glBindVertexArray(0);

				glDepthFunc( GL_LESS ); 


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

