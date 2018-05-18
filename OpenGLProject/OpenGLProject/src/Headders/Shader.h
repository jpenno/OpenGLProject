#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

#include "VertexBufferLayout.h"


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_fielPath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_uniformLocationCach;

public:
	VertexBufferLayout Vbl;

	Shader(const std::string& fielPath);
	~Shader();

	void Bind() const;
	void Unbidn() const;

	// Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, glm::vec3 value);
	void SetUniform1i(const std::string& name, int value);
	void SetuniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string &source);
	int GetUniformLocation(const std::string& name);
	ShaderProgramSource PassShader(const std::string& filepath);
};