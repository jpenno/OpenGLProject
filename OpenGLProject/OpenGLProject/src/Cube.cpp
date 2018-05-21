#include "Cube.h"

#include <glm/ext.hpp>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "Renderer.h"


Cube::Cube(glm::vec3 a_pos, float scale, std::vector<std::string> fielPaths)
{
	// set the position of the cube
	m_matPos[3][0] = a_pos.x;
	m_matPos[3][1] = a_pos.y;
	m_matPos[3][2] = a_pos.z;

	// set the scale of the cube
	m_matPos[0][0] = scale;
	m_matPos[1][1] = scale;
	m_matPos[2][2] = scale;

	 float verts [] {
		// front
		-0.5f, -0.5f,  0.5f, // 0
		 0.5f, -0.5f,  0.5f, // 1
		 0.5f,  0.5f,  0.5f, // 2
	    -0.5f,  0.5f,  0.5f, // 3

		// Right Face
		 0.5f, -0.5f,  0.5f, // 4
		 0.5f, -0.5f, -0.5f, // 5
		 0.5f,  0.5f, -0.5f, // 6
		 0.5f,  0.5f,  0.5f, // 7

		// Left Face
		-0.5f, -0.5f,  0.5f, // 8
		-0.5f, -0.5f, -0.5f, // 9
		-0.5f,  0.5f, -0.5f, // 10
		-0.5f,  0.5f,  0.5f, // 11

		// Back face
	 	 0.5f, -0.5f, -0.5f, // 12
		-0.5f, -0.5f, -0.5f, // 13
		-0.5f,  0.5f, -0.5f, // 14
		 0.5f,  0.5f, -0.5f, // 15

		// Top face
		 0.5f,  0.5f,  0.5f, // 16
		 0.5f,  0.5f, -0.5f, // 17
		-0.5f,  0.5f, -0.5f, // 18
		-0.5f,  0.5f,  0.5f, // 19

		// Bottom face
	     0.5f, -0.5f,  0.5f, // 20
	     0.5f, -0.5f, -0.5f, // 21
	    -0.5f, -0.5f, -0.5f, // 22
	    -0.5f, -0.5f,  0.5f  // 23
	};

	 unsigned char indiceTesting[36]{
		0, 1, 2,		0, 2, 3,	// front face
		4, 5, 6,		4, 6, 7,	// Right face
		10, 9, 8,		11, 10, 8,	// Left face 
		12, 13, 14,		12, 14, 15,	// Back face
		16, 17, 18,		16, 18, 19, // Top face
		22, 21, 20,		22, 20, 23	// Bottom face
	};

	m_va = new VertexArray();
	m_vb = new VertexBuffer(verts,  sizeof(verts));

	m_shader = new Shader("data/shaders/CubeMap.shader");
	m_shader->Bind();

	m_layout = new VertexBufferLayout();
	m_layout->Push<float>(3);
	//m_layout->Push<float>(2);
	m_va->AddBuffer(*m_vb, *m_layout);

	m_ib = new IndexBuffer(indiceTesting, 36);

	m_texture = new Texture();
	m_texture->LoadTextureCubeMap(fielPaths);
}

Cube::~Cube()
{
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
	glm::mat4 pos = projMat * m_matPos;
	m_shader->SetuniformMat4f("u_Pos", pos);

	GLCall(glDrawElements(GL_TRIANGLES, m_ib->GetCount(), GL_UNSIGNED_BYTE, nullptr));
}
