#include "SkyBox.h"

#include <glm/ext.hpp>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "Renderer.h"

#include <iostream>

SkyBox::SkyBox(const std::vector<std::string>& filePaths, const glm::mat4& a_projection, const glm::mat4& a_viweMat,
	float nearPlane /*= 0.1f*/, float farPlane /*= 1000.0f*/) :
	m_projection(a_projection),
	m_viweMat(a_viweMat)
{
	float verts[] {
		// front
		-1.0f, -1.0f,  1.0f, // 0
		 1.0f, -1.0f,  1.0f, // 1
		 1.0f,  1.0f,  1.0f, // 2
		-1.0f,  1.0f,  1.0f, // 3

		// back
		-1.0f, -1.0f, -1.0f, // 4
		 1.0f, -1.0f, -1.0f, // 5
		 1.0f,  1.0f, -1.0f, // 6
		-1.0f,  1.0f, -1.0f, // 7
	};

	unsigned char indices[] {
		2,  1,  0,	  3,  2,  0, // front face
		6,  5,  1,	  2,  6,  1, // Right face
		0,  4,  7,	  0,  7,  3, // Left face
		4,  5,  6,	  4,  6,  7, // Back face
		6,  2,  3,	  7,  6,  3, // Top face
		0,  1,  5,    0,  5,  4	 // Bottom face
	};

	m_va = new VertexArray();

	m_shader = new Shader("data/shaders/skyBox.shader");
	m_shader->Bind();

	m_vb = new VertexBuffer(verts, sizeof(verts));

	m_layout = new VertexBufferLayout();
	m_layout->Push<float>(3);
	m_va->AddBuffer(*m_vb, *m_layout);

	m_ib = new IndexBuffer(indices, 36);

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

	if (m_ib != nullptr)
		delete m_ib;
}

void SkyBox::Draw(glm::mat4 a_viweMat)
{
	GLCall(glDepthFunc(GL_LEQUAL));
	m_va->Bind();

	m_shader->Bind();

	// zero out the position of the view matrx to center the sky box around the camera
	a_viweMat[3][0] = 0;
	a_viweMat[3][1] = 0;
	a_viweMat[3][2] = 0;

	glm::mat4 pos = m_projection * a_viweMat;

	m_shader->SetuniformMat4f("u_Pos", pos);

	m_texture->Bind();

	GLCall(glDrawElements(GL_TRIANGLES, m_ib->GetCount(), GL_UNSIGNED_BYTE, nullptr));
	glDepthFunc(GL_LESS);
}

