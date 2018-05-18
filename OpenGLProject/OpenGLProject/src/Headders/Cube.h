#pragma once
#include "glm/glm.hpp"
#include "Renderer.h"

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

	float* m_verts = nullptr;
	unsigned int* m_indices = nullptr;

	Renderer m_renderer;

	// OpenGl data
	VertexArray*            m_va = nullptr;
	VertexBuffer*           m_vb = nullptr;
	Shader*             m_shader = nullptr;
	VertexBufferLayout* m_layout = nullptr;
	IndexBuffer*            m_ib = nullptr;
	Texture*           m_texture = nullptr;

public:
	Cube(glm::vec3 a_pos, float scale);
	~Cube();


	void Draw(glm::mat4 projMat);

};