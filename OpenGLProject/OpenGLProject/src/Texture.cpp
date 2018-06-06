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
	// set the texture type
	m_textureType = Type::TEXTURE2D;
	stbi_set_flip_vertically_on_load(1);
	// load the image in to a char array
	m_localBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_BP, 4);

	// gen the texture
	GLCall(glGenTextures(1, &m_rendererID));

	// bind the texture
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

	// set the texture parameters
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//bind the image to the texture
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
	
	// unbind the texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	// free the image if the image is there
	if (m_localBuffer)
		stbi_image_free(m_localBuffer);

}

void Texture::LoadTextureCubeMap(const std::vector<std::string>& fielPaths)
{
	// set the texture type
	m_textureType = Type::TEXUTRE_CUBEMAP;

	// gen the texture
	GLCall(glGenTextures(1, &m_rendererID));

	// bind the texture
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID));


	for (int i = 0; i < fielPaths.size(); i++) {
		// load the image form file
		m_localBuffer = stbi_load(fielPaths[i].c_str(), &m_width, &m_height, &m_BP, 4);

		// push the image to the graphics card and bind it to the cube map
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8,
			m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));

		// free the image if the image is there
		if (m_localBuffer)
			stbi_image_free(m_localBuffer);
	}

	// set the texture parameters
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	// unbind the texture
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	switch (m_textureType)
	{
	case Texture::Type::TEXTURE2D:
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		// bind the texture2D
		GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
		break;
	case Texture::Type::TEXUTRE_CUBEMAP:
		// bind the cube map texture
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID);
		break;
	default:
		break;
	}
}

void Texture::Unbind() const
{
	switch (m_textureType)
	{
	case Texture::Type::TEXTURE2D:
		// unbind the texture2D
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		break;
	case Texture::Type::TEXUTRE_CUBEMAP:
		// unbind the cube map texture
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
		break;
	default:
		break;
	}
}

// AIE load texture function
bool Texture::load(const char* filename) {

	if (m_rendererID != 0) {
		glDeleteTextures(1, &m_rendererID);
		m_rendererID = 0;
		m_width = 0;
		m_height = 0;
	}

	int x = 0, y = 0, comp = 0;
	m_localBuffer = stbi_load(filename, &x, &y, &comp, STBI_default);

	if (m_localBuffer != nullptr) {
		glGenTextures(1, &m_rendererID);
		glBindTexture(GL_TEXTURE_2D, m_rendererID);
		switch (comp) {
		case STBI_grey:
			m_format = RED;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, x, y,
				0, GL_RED, GL_UNSIGNED_BYTE, m_localBuffer);
			break;
		case STBI_grey_alpha:
			m_format = RG;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, x, y,
				0, GL_RG, GL_UNSIGNED_BYTE, m_localBuffer);
			break;
		case STBI_rgb:
			m_format = RGB;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y,
				0, GL_RGB, GL_UNSIGNED_BYTE, m_localBuffer);
			break;
		case STBI_rgb_alpha:
			m_format = RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y,
				0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
			break;
		default:	break;
		};
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_width = (unsigned int)x;
		m_height = (unsigned int)y;
		return true;
	}
	return false;
}

