#pragma once
#include <string>
#include <vector>



class Texture
{
private:
	enum class Type {
		TEXTURE2D, TEXUTRE_CUBEMAP
	};

	Type m_textureType;

	unsigned int m_rendererID;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_BP;


public:
	Texture();
	~Texture();

	void LoadTexture2D(const std::string& filePath);
	void LoadTextureCubeMap(const std::vector<std::string>& fielPaths);

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
};