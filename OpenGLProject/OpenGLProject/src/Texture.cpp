#include "Texture.h"
#include "stb_image/stb_image.h"

#include "Renderer.h"

#include <iostream>

Texture::Texture()
	:m_rendererID(0), m_filePath(""), 
	m_localBuffer(nullptr), m_width(0), m_height(0), m_BP(0)
{}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_rendererID))
}

void Texture::LoadTexture2D(const std::string& filePath)
{
	m_textureType = Type::TEXTURE2D;
	stbi_set_flip_vertically_on_load(1);
	m_localBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_BP, 4);

	GLCall(glGenTextures(1, &m_rendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_localBuffer)
		stbi_image_free(m_localBuffer);

}

void Texture::LoadTextureCubeMap(const std::vector<std::string>& fielPaths)
{
	m_textureType = Type::TEXUTRE_CUBEMAP;

	GLCall(glGenTextures(1, &m_rendererID));

	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID));


	for (int i = 0; i < fielPaths.size(); i++) {

		//stbi_set_flip_vertically_on_load(1);
		m_localBuffer = stbi_load(fielPaths[i].c_str(), &m_width, &m_height, &m_BP, 4);

		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8,
			m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));

		if (m_localBuffer)
			stbi_image_free(m_localBuffer);
	}

	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	switch (m_textureType)
	{
	case Texture::Type::TEXTURE2D:
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
		break;
	case Texture::Type::TEXUTRE_CUBEMAP:
		std::cout << "TEXUTRE_CUBEMAP" << std::endl;
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID);
		break;
	default:
		break;
	}
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
