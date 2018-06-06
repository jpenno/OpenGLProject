#include "Cube.h"

#include <glm/ext.hpp>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "Renderer.h"
// set cube static verts
float Cube::verts[24]{
	// front
	-0.5f, -0.5f,  0.5f, // 0
	 0.5f, -0.5f,  0.5f, // 1
	 0.5f,  0.5f,  0.5f, // 2
	-0.5f,  0.5f,  0.5f, // 3

	// back
	-0.5f, -0.5f, -0.5f, // 4
	 0.5f, -0.5f, -0.5f, // 5
	 0.5f,  0.5f, -0.5f, // 6
	-0.5f,  0.5f, -0.5f, // 7
};

// set cube static indices
unsigned char Cube::indices[36]{
	0,  1,  2,	    0,  2,  3,	// front face
	1,  5,  6,	    1,  6,  2,	// Right face
	7,  4,  0,	    3,  7,  0,	// Left face
	6,  5,  4,	    7,  6,  4,	// Back face
	3,  2,  6,	    3,  6,  7,  // Top face
	5,  1,  0,  	4,  5,  0	// Bottom face
};

Cube::Cube(const std::vector<std::string>& fielPaths, const glm::vec3& a_pos, const glm::mat4& a_projection, const float scale /*= 1*/)
	:m_projection(a_projection)
{
	// set the position of the cube
	m_matPos[3][0] = a_pos.x;
	m_matPos[3][1] = a_pos.y;
	m_matPos[3][2] = a_pos.z;
	 
	// set the scale of the cube
	m_matPos[0][0] = scale;
	m_matPos[1][1] = scale;
	m_matPos[2][2] = scale;

	// make a vertex array
	m_va = new VertexArray();

	// set the vertex buffer
	m_vb = new VertexBuffer(verts,  sizeof(verts));

	// set the shader
	m_shader = new Shader("data/shaders/CubeMap.shader");

	// set the vertex buffer layout
	m_layout = new VertexBufferLayout();
	m_layout->Push<float>(3);
	m_va->AddBuffer(*m_vb, *m_layout);

	// set the index buffer
	m_ib = new IndexBuffer(indices, 36);

	// set the texture
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

void Cube::Draw(const glm::mat4& projMat)
{
	// bind the vertex array
	m_va->Bind();

	// bind the shader
	m_shader->Bind();

	// bind the texture
	m_texture->Bind();

	// set the projection view matrix
	glm::mat4 pvm = projMat * m_matPos;
	m_shader->SetuniformMat4f("u_Pos", pvm);

	// draw
	GLCall(glDrawElements(GL_TRIANGLES, m_ib->GetCount(), GL_UNSIGNED_BYTE, nullptr));
}
