#pragma once
#include <string>
#include <vector>



class Texture
{
private:
	enum class Type {
		TEXTURE2D, TEXUTRE_CUBEMAP
	};

	enum Format : unsigned int {
		RED = 1,
		RG,
		RGB,
		RGBA
	};

	unsigned int m_format;


	Type m_textureType;

	unsigned int m_rendererID;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_BP;


public:
	Texture();
	~Texture();

	// AIE load texture function
	bool load(const char* filename);


	void LoadTexture2D(const std::string& filePath);
	void LoadTextureCubeMap(const std::vector<std::string>& fielPaths);

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline unsigned int getHandle() const { return m_rendererID; }
	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
};