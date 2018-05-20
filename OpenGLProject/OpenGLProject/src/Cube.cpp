#include "Cube.h"

#include <glm/ext.hpp>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "Renderer.h"


Cube::Cube(glm::vec3 a_pos, float scale)
{
	m_verts = new float [6 * 4 * 5] {
		// front
		a_pos.x + -0.5f, a_pos.y + -0.5f, a_pos.z + 0.5f, 0.0, 0.0, // 0
		a_pos.x + 0.5f, a_pos.y + -0.5f,  a_pos.z + 0.5f, 1.0, 0.0, // 1
		a_pos.x + 0.5f, a_pos.y + 0.5f,   a_pos.z + 0.5f, 1.0, 1.0, // 2
		a_pos.x + -0.5f, a_pos.y + 0.5f,  a_pos.z + 0.5f, 0.0, 1.0, // 3

		// Right Face
		a_pos.x + 0.5f,  a_pos.y + -0.5f, a_pos.z + 0.5f, 0.0, 0.0, // 4
		a_pos.x + 0.5f,  a_pos.y + -0.5f, a_pos.z + -0.5f, 1.0, 0.0, // 5
		a_pos.x + 0.5f,  a_pos.y + 0.5f,  a_pos.z + -0.5f, 1.0, 1.0, // 6
		a_pos.x + 0.5f,  a_pos.y + 0.5f,  a_pos.z + 0.5f, 0.0, 1.0, // 7

		// Left Face
		a_pos.x + -0.5f, a_pos.y + -0.5f,a_pos.z + 0.5f, 0.0, 0.0, // 8
		a_pos.x + -0.5f, a_pos.y + -0.5f,a_pos.z + -0.5f, 1.0, 0.0, // 9
		a_pos.x + -0.5f, a_pos.y + 0.5f, a_pos.z + -0.5f, 1.0, 1.0, // 10
		a_pos.x + -0.5f, a_pos.y + 0.5f, a_pos.z + 0.5f, 0.0, 1.0, // 11

		// Back face
		a_pos.x + 0.5f,   a_pos.y + -0.5f,a_pos.z + -0.5f, 1.0, 0.0, // 12
		a_pos.x + -0.5f,  a_pos.y + -0.5f,a_pos.z + -0.5f, 0.0, 0.0, // 13
		a_pos.x + -0.5f,  a_pos.y + 0.5f, a_pos.z + -0.5f, 0.0, 1.0, // 14
		a_pos.x + 0.5f,   a_pos.y + 0.5f, a_pos.z + -0.5f, 1.0, 1.0, // 15

		// Top face					   
		a_pos.x + 0.5f,  a_pos.y + 0.5f,a_pos.z + 0.5f, 1.0, 1.0, // 16
		a_pos.x + 0.5f,  a_pos.y + 0.5f,a_pos.z + -0.5f, 1.0, 0.0, // 17
		a_pos.x + -0.5f, a_pos.y + 0.5f,a_pos.z + -0.5f, 0.0, 0.0, // 18
		a_pos.x + -0.5f, a_pos.y + 0.5f,a_pos.z + 0.5f, 0.0, 1.0, // 19

		// Bottom face
		a_pos.x + 0.5f,   a_pos.y + -0.5f,a_pos.z + 0.5f, 1.0, 1.0, // 20
		a_pos.x + 0.5f,   a_pos.y + -0.5f,a_pos.z + -0.5f, 1.0, 0.0, // 21
		a_pos.x + -0.5f,  a_pos.y + -0.5f,a_pos.z + -0.5f, 0.0, 0.0, // 22
		a_pos.x + -0.5f,  a_pos.y + -0.5f,a_pos.z + 0.5f, 0.0, 1.0  // 23
	};


	m_indices = new unsigned int[36] {
		0, 1, 2,		0, 2, 3,	// front face
		4, 5, 6,		4, 6, 7,	// Right face
		10, 9, 8,		11, 10, 8,	// Left face 
		12, 13, 14,		12, 14, 15,	// Back face
		16, 17, 18,		16, 18, 19, // Top face
		22, 21, 20,		22, 20, 23	// Bottom face
	};

	m_va = new VertexArray();

	m_vb = new VertexBuffer(m_verts, 6 * 4 * 5 * sizeof(float));

	m_shader = new Shader("data/shaders/Basic.shader");
	m_shader->Bind();

	glm::mat4 orthoProj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

	m_layout = new VertexBufferLayout();
	m_layout->Push<float>(3);
	m_layout->Push<float>(2);
	m_va->AddBuffer(*m_vb, *m_layout);

	m_ib = new IndexBuffer(m_indices, 36);

	m_texture = new Texture();
	m_texture->LoadTexture2D("data/Textures/4-2-car-png-hd.png");
}

Cube::~Cube()
{
	if (m_verts != nullptr)
		delete m_verts;

	if (m_indices != nullptr)
		delete m_indices;

	if (m_va != nullptr)
		delete m_va;

	if (m_vb != nullptr)
		delete m_vb;

	if (m_shader != nullptr)
		delete m_shader;
	
	if (m_layout != nullptr)
		delete m_layout;

	if (m_ib != nullptr)
		delete m_ib;

	if (m_texture != nullptr)
		delete m_texture;
}

void Cube::Draw(glm::mat4 projMat)
{
	m_va->Bind();
	m_shader->Bind();
	m_texture->Bind();
	m_shader->SetuniformMat4f("u_MVP", projMat);
	GLCall(glDrawElements(GL_TRIANGLES, m_ib->GetCount(), GL_UNSIGNED_INT, nullptr));
}
