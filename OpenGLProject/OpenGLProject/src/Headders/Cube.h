#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <string>

class VertexArray;
class VertexBuffer;
class VertexBufferLayout;
class IndexBuffer;
class Shader;
class Texture;

class Cube
{
private:
	glm::vec4 m_position;
	glm::mat4 m_matPos;
	glm::mat4 m_projection;


	// OpenGl data
	VertexArray*            m_va = nullptr;
	VertexBuffer*           m_vb = nullptr;
	VertexBufferLayout* m_layout = nullptr;
	IndexBuffer*            m_ib = nullptr;

	Shader*             m_shader = nullptr;
	Texture*           m_texture = nullptr;




	static float verts[24];
	static unsigned char indices[36];

public:
	Cube(const std::vector<std::string>& fielPaths, const glm::vec3& a_pos, const glm::mat4& a_projection, const float scale = 1 );
	~Cube();

	void Draw(const glm::mat4& projMat);
};