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
	Shader*             m_shader = nullptr;
	VertexBufferLayout* m_layout = nullptr;
	IndexBuffer*            m_ib = nullptr;
	Texture*           m_texture = nullptr;

public:
	Cube(glm::vec3 a_pos, float scale, std::vector<std::string> fielPaths);
	~Cube();


	void Draw(glm::mat4 projMat);

};