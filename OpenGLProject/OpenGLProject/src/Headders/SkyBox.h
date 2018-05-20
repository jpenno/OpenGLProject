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
	Shader*             m_shader = nullptr;
	VertexBufferLayout* m_layout = nullptr;
	Texture*           m_texture = nullptr;


public:
	SkyBox(std::vector<std::string>& filePaths, float fov, float screenWidth, float screenHeight, 
		glm::mat4 m_viweMat, float nearPlane = 0.1f, float farPlane = 1000.0f);

	~SkyBox();

	void Draw(glm::mat4 m_viweMat);

};