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

layout(location = 0) out vec4 color;

in vec3 TexCoords;

uniform vec4 u_Color;
uniform samplerCube skybox;

void main()
{
	color = texture(skybox, TexCoords);
};
