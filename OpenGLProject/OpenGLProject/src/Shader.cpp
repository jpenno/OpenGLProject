#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& fielPath) :
	m_fielPath(fielPath), m_RendererID(0)
{
	// get the vertex and frag shaders in to c++ strings
	ShaderProgramSource source = PassShader(fielPath);
	// create the shader
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbidn() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 value)
{
	GLCall(glUniform3f(GetUniformLocation(name), value[0], value[1], value[2]));
}

void Shader::SetUniform1f(const std::string & name, float v0)
{
	GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetuniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetuniformMat3f(const std::string& name, const glm::mat3& matrix)
{
	GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_uniformLocationCach.find(name) != m_uniformLocationCach.end())
		return m_uniformLocationCach[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "warning:uniform " << name << " doesn't exist!" << std::endl;

	m_uniformLocationCach[name] = location;
	return location;
}

ShaderProgramSource Shader::PassShader(const std::string& filepath)
{
	// open the file the shader is in
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	// read the file the vert and frag shader are in
	while (getline(stream, line))
	{
		// check the line for shader
		if (line.find("#shader") != std::string::npos)
		{
			// check if it is a vertex shader
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX; // set shader type to vertex

			// check if it is a fragment shader
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT; // set shader type to fragment
		}
		else
		{
			// read shaders in to the string stream
			ss[(int)type] << line << '\n';
		}
	}
	// return the shaders as strings
	return { ss[0].str(), ss[1].str() };
}


unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
	// make a program for the vertex and fragment shaders
	unsigned int program = glCreateProgram();
	// complie the vertex shader
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	// compile the fragment shader
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// attach vertex shader to the program
	GLCall(glAttachShader(program, vs));
	// attach fragment shadert to the program
	GLCall(glAttachShader(program, fs));
	// link the program
	GLCall(glLinkProgram(program));
	// validate the program
	GLCall(glValidateProgram(program));

	// delete the vertex and fragment shaders
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
	// create a shader
	unsigned int id = glCreateShader(type);
	// compile the shader
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	// check if the shader compiled
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		// get the error from the shader not comiling
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") << "shader" << std::endl;
		// print the error message
		std::cout << message << std::endl;

		// delete the shader
		GLCall(glDeleteShader(id));
		return 0;
	}
	// return the compiled shader
	return id;
}