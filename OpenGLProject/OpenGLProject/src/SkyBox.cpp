#include "SkyBox.h"

#include <glm/ext.hpp>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "Renderer.h"

SkyBox::SkyBox(std::vector<std::string>& filePaths, glm::mat4 a_projection, glm::mat4 viweMat,
				float nearPlane /*= 0.1f*/, float farPlane /*= 1000.0f*/)
{
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

	m_viweMat = viweMat;
	m_projection = a_projection;

	m_va = new VertexArray();

	m_shader = new Shader("data/shaders/skyBox.shader");
	m_shader->Bind();

	m_vb = new VertexBuffer(skyboxVertices, sizeof(skyboxVertices));

	m_layout = new VertexBufferLayout();
	m_layout->Push<float>(3);
	m_va->AddBuffer(*m_vb, *m_layout);

	m_texture = new Texture();
	m_texture->LoadTextureCubeMap(filePaths);
}

SkyBox::~SkyBox()
{
	if (m_va != nullptr)
		delete m_va;

	if (m_vb != nullptr)
		delete m_vb;

	if (m_shader != nullptr)
		delete m_shader;

	if (m_layout != nullptr)
		delete m_layout;

	if (m_texture != nullptr)
		delete m_texture;
}

void SkyBox::Draw(glm::mat4 a_viweMat)
{
	GLCall(glDepthFunc(GL_LEQUAL));
	m_shader->Bind();
	glm::mat3 viweMat = glm::mat3(a_viweMat);
	m_shader->SetuniformMat4f("view", viweMat);
	m_shader->SetuniformMat4f("projection", m_projection);
	m_va->Bind();

	m_texture->Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	glDepthFunc(GL_LESS);
}

