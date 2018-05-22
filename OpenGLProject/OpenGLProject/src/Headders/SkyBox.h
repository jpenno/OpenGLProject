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


class SkyBox {
private:
	glm::mat4 m_projection;
	glm::mat4 m_viweMat;

	// OpenGl data
	VertexArray *           m_va = nullptr;
	VertexBuffer*           m_vb = nullptr;
	IndexBuffer*            m_ib = nullptr;
	Shader*             m_shader = nullptr;
	VertexBufferLayout* m_layout = nullptr;
	Texture*           m_texture = nullptr;


public:
	SkyBox(const std::vector<std::string>& filePaths, const glm::mat4& a_projection, const glm::mat4& a_viweMat,
		 float nearPlane = 0.1f,  float farPlane = 1000.0f);

	~SkyBox();

	void Draw(glm::mat4 m_viweMat);

};