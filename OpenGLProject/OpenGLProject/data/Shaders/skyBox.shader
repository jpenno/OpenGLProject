#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;


void main()
{
	vec4 pos = projection * view * vec4(position, 1.0);
	gl_Position = pos.xyww;
	TexCoords = position;
}

#shader fragment
#version 330 core

in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{
    color = texture(skybox, TexCoords);
}