#include "App.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Gizmos.h"

#include "Renderer.h"
#include "Shader.h"

#include "Cube.h"
#include "SkyBox.h"
#include "OBJMesh\OBJMesh.h"

#include "Input.h"

#include <string>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;


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

	// enable opengl blend
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// start input manager
	Input::create();

	return true;
}

bool App::Startup() {
	Gizmos::create(10000, 10000, 10000, 10000);

	// set camera
	m_camera.setPos(vec3(10, 10, 10));
	m_camera.setProjectionView(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
	m_camera.Lookat(vec3(0, 0, 0));
	m_gameOver = false;

	// set Direction light
	m_dirLight.diffuse = { 1, 1, 0 };
	m_dirLight.specular = { 1, 1, 1 };
	m_dirLight.ambientLight = { 0.25f, 0.25f, 0.25f };

	// set point light
	m_pointLight.pos = { 0, 3, 2 };
	m_pointLight.diffuse = { 0, 1, 0 };
	m_pointLight.specular = { 1, 1, 1 };
	m_pointLight.ambientLight = { 0.25f, 0.25f, 0.25f };
	m_pointLight.constant = 1.0f;
	m_pointLight.linear = 0.09f;
	m_pointLight.quadratic = 0.032f;

	// load shders
	m_lightingShader = new Shader("data/shaders/mutipleLights.shader");

	// set up the file paths for the cube textures
	std::vector<std::string> cubeTexturesPaths;
	cubeTexturesPaths.emplace_back("data/Textures/lilacisles_ft.png");  // front
	cubeTexturesPaths.emplace_back("data/Textures/lilacisles_bk.png");	// back
	cubeTexturesPaths.emplace_back("data/Textures/lilacisles_up.png");	// top
	cubeTexturesPaths.emplace_back("data/Textures/lilacisles_dn.png");	// bottom
	cubeTexturesPaths.emplace_back("data/Textures/lilacisles_rt.png");	// right
	cubeTexturesPaths.emplace_back("data/Textures/lilacisles_lf.png");	// left

	// set cube
	m_cube = new Cube(cubeTexturesPaths, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), m_camera.GetProjectionView());

	// set skybox
	m_skyBox = new SkyBox(cubeTexturesPaths, m_camera.GetProjectionView(), m_camera.GetViewMat());

	// scale var for the scale of the obj
	float scale;
	// load soulsper obj
	m_soulsper = new aie::OBJMesh();
	m_soulsper->load("data/soulspear/soulspear.obj", true, true);
	// set the matrix for m_soulsper
	scale = 0.5;
	soulspherPos = {
		scale,0,0,0,
		0,scale,0,0,
		0,0,scale,0,
		0,1,0,1
	};

	// load nanosuit obj
	m_nanosuitMesh = new aie::OBJMesh();
	m_nanosuitMesh->load("data/nanosuit/nanosuit.obj", true, true);
	// set the matrix for m_nanosuitMesh
	scale = 0.25;
	nanosuitPos = {
		scale,0,0,0,
		0,scale,0,0,
		0,0,scale,0,
		5,1,0,1
	};

	return true;
}

void App::shutdown()
{
	Gizmos::destroy();

	if (m_soulsper != nullptr)
		delete m_soulsper;

	if (m_nanosuitMesh != nullptr)
		delete m_nanosuitMesh;

	if (m_lightingShader != nullptr)
		delete m_lightingShader;

	if (m_cube != nullptr)
		delete m_cube;

	if (m_skyBox != nullptr)
		delete m_skyBox;

	glfwDestroyWindow(window);
	glfwTerminate();

	Input::destory();
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
		
		while (!m_gameOver)
		{
			// update delta time
			currTime = (float)glfwGetTime();
			deltaTime = currTime - prevTime;
			prevTime = currTime;
			// rotate light
			m_dirLight.direction = glm::normalize(vec3(glm::cos(currTime * 2),
				glm::sin(currTime * 2), 0));

			// clear input
			Input::getInstance()->clearStatus();

			// update window events (input etc)
			glfwPollEvents();

			update(deltaTime);

			// clear the screen
			clearScreen();

			// draw cube
			m_cube->Draw(m_camera.GetProjectionView());

			// draw skybox
			//m_skyBox->Draw(m_camera.GetViewMat());

			// use the lighting shader
			m_lightingShader->Bind();

			// set the camera position in the shader
			m_lightingShader->SetUniform3f("cameraPosition", m_camera.GetPos());

			// set direction light in the shader
			m_lightingShader->SetUniform3f("dirLight.ambient", m_dirLight.ambientLight);
			m_lightingShader->SetUniform3f("dirLight.diffuse", m_dirLight.diffuse);
			m_lightingShader->SetUniform3f("dirLight.specular", m_dirLight.specular);
			m_lightingShader->SetUniform3f("dirLight.direction", m_dirLight.direction);

			// set point light in the shader
			m_lightingShader->SetUniform3f("pointLight.ambient", m_pointLight.ambientLight);
			m_lightingShader->SetUniform3f("pointLight.diffuse", m_pointLight.diffuse);
			m_lightingShader->SetUniform3f("pointLight.specular", m_pointLight.specular);
			m_lightingShader->SetUniform3f("pointLight.position", m_pointLight.pos);
			m_lightingShader->SetUniform1f("pointLight.constant", m_pointLight.constant);
			m_lightingShader->SetUniform1f("pointLight.linear", m_pointLight.linear);
			m_lightingShader->SetUniform1f("pointLight.quadratic", m_pointLight.quadratic);


			// get the position view matrix for the nanosuit
			glm::mat4 pvm = m_camera.GetProjectionView() * nanosuitPos;
			// bind the position view matrix
			m_lightingShader->SetuniformMat4f("ProjectionViewModel", pvm);
			// bind the model matrix
			m_lightingShader->SetuniformMat4f("ModelMatrix", nanosuitPos);
			// bind the normal matrix
			m_lightingShader->SetuniformMat3f("NormalMatrix",
				glm::inverseTranspose(glm::mat3(nanosuitPos)));

			// draw nanosuite 
			m_nanosuitMesh->draw();

			// get the position view matrix for the soulspher
			pvm = m_camera.GetProjectionView() * soulspherPos;
			// bind the position view matrix
			m_lightingShader->SetuniformMat4f("ProjectionViewModel", pvm);
			// bind the model matrix
			m_lightingShader->SetuniformMat4f("ModelMatrix", soulspherPos);	
			// bind the normal matrix
			m_lightingShader->SetuniformMat3f("NormalMatrix",
				glm::inverseTranspose(glm::mat3(soulspherPos)));
			// draw soul sphere
			m_soulsper->draw();

			draw();

			if (glfwWindowShouldClose(window) == GLFW_TRUE)
				quit();
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

	// move the point light 
	m_pointLight.pos.y += 1 * deltaTime;
	if (m_pointLight.pos.y > 10)
		m_pointLight.pos.y = 0;
}

void App::draw() 
{
	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1));

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	// draw grid
	for (int i = 0; i < 21; ++i) 
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);

		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// draw the light as a sphere
	Gizmos::addSphere(m_pointLight.pos, 0.25f, 10, 10, glm::vec4(m_pointLight.diffuse, 1.0f));


	Gizmos::draw(m_camera.GetProjectionView());
	glfwSwapBuffers(window);
}

void App::clearScreen() 
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

