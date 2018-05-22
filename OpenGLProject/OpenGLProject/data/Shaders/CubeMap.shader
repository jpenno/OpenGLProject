#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 u_Pos;

void main()
{
	gl_Position = u_Pos * vec4(position, 1.0);

	TexCoords = position;
};

#shader fragment
#version 330 core
in vec3 TexCoords;

uniform samplerCube skybox;

out vec4 color;

void main()
{
	color = texture(skybox, TexCoords);
};
